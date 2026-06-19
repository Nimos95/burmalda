#include "game_controller.h"
#include "random_engine.h"
#include <stdexcept>

static RandomEngine s_rng;

GameController::GameController(std::shared_ptr<PayTable> payTable)
    : m_payTable(payTable)
    , m_balance(1000)
    , m_lastWin(0)
    , m_totalWins(0)
    , m_totalSpins(0)
{
}

void GameController::initGame() {
    m_reels.clear();
    const auto& config = m_payTable->getReelsConfig();
    for (const auto& reelSymbols : config) {
        m_reels.emplace_back(reelSymbols);
    }

    for (auto& reel : m_reels) {
        reel.spin(0);
    }

    m_currentSymbols.clear();
    for (const auto& reel : m_reels) {
        m_currentSymbols.push_back(reel.getVisibleSymbols(reel.getPosition()));
    }

    m_balance = 1000;
    m_lastWin = 0;
    m_totalWins = 0;
    m_totalSpins = 0;
    m_winningLines.clear();
    m_linePayouts.clear();
}

void GameController::spin(int betPerLine) {
    // ✅ Проверяем баланс
    int totalBet = betPerLine * m_payTable->getLines().size();
    if (m_balance < totalBet) {
        throw std::runtime_error("Insufficient balance!");
    }

    // ✅ Списываем ставку
    m_balance -= totalBet;
    m_totalSpins++;

    // Прокручиваем барабаны
    for (auto& reel : m_reels) {
        int steps = s_rng.generate(1, static_cast<int>(reel.size()));
        reel.spin(steps);
    }

    // Получаем символы
    m_currentSymbols.clear();
    for (const auto& reel : m_reels) {
        m_currentSymbols.push_back(reel.getVisibleSymbols(reel.getPosition()));
    }

    // Проверяем линии
    m_winningLines.clear();
    m_linePayouts.clear();
    m_lastWin = 0;

    for (const auto& line : m_payTable->getLines()) {
        std::vector<std::string> lineSymbols;
        bool valid = true;

        for (size_t reelIdx = 0; reelIdx < line.positions.size(); ++reelIdx) {
            if (reelIdx >= m_currentSymbols.size()) {
                valid = false;
                break;
            }

            const auto& pos = line.positions[reelIdx];
            if (pos.empty()) {
                valid = false;
                break;
            }

            int row = pos[0];
            if (row < 0 || row >= 3) {
                valid = false;
                break;
            }

            if (row >= static_cast<int>(m_currentSymbols[reelIdx].size())) {
                valid = false;
                break;
            }

            lineSymbols.push_back(m_currentSymbols[reelIdx][row]);
        }

        if (!valid || lineSymbols.empty()) {
            continue;
        }

        std::string first = lineSymbols[0];
        int count = 1;
        for (size_t i = 1; i < lineSymbols.size(); ++i) {
            if (lineSymbols[i] == first) {
                count++;
            }
            else {
                break;
            }
        }

        if (count >= 3) {
            int payout = m_payTable->getPayout(first, count, betPerLine);
            if (payout > 0) {
                m_winningLines.push_back(line.id);
                m_linePayouts.push_back(payout);
                m_lastWin += payout;
            }
        }
    }

    // ✅ ДОБАВЛЯЕМ ВЫИГРЫШ К БАЛАНСУ
    m_balance += m_lastWin;
    m_totalWins += m_lastWin;
}

std::vector<std::vector<std::string>> GameController::getCurrentSymbols() const {
    return m_currentSymbols;
}

const std::vector<PayLine>& GameController::getLines() const {
    return m_payTable->getLines();
}