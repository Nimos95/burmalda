#include "random_engine.h"
#include "reel.h"
#include "paytable.h"
#include <iostream>
#include <iomanip>
#include <memory>
#include <map>

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  🎰 BURMALDA SLOT MACHINE DEMO  🎰" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Загружаем конфигурацию
    std::cout << "Loading configuration..." << std::endl;
    auto payTable = std::make_shared<PayTable>();

    if (!payTable->loadFromFile("config/reels.json", "config/paytable.json")) {
        std::cerr << "Failed to load configs!" << std::endl;
        return 1;
    }

    std::cout << "✅ Config loaded!" << std::endl;
    std::cout << "   Lines: " << payTable->getLines().size() << std::endl;
    std::cout << "   Symbols: " << payTable->getSymbols().size() << std::endl;
    std::cout << std::endl;

    // Создаём барабаны
    std::vector<Reel> reels;
    for (const auto& reelConfig : payTable->getReelsConfig()) {
        reels.emplace_back(reelConfig);
    }

    // Генератор случайных чисел
    RandomEngine rng(42);

    // Настройки игры
    int balance = 1000;
    int betPerLine = 10;
    int totalBet = betPerLine * payTable->getLines().size();

    std::cout << "Starting game with balance: " << balance << " coins" << std::endl;
    std::cout << "Bet per spin: " << totalBet << " coins" << std::endl;
    std::cout << std::endl;

    // --- НОРМАЛЬНЫЕ СПИНЫ ---
    std::cout << "--- NORMAL SPINS (30 spins) ---" << std::endl;
    std::cout << std::endl;

    // Статистика
    int totalSpins = 0;
    int totalWins = 0;
    int totalWinAmount = 0;
    int maxWin = 0;

    const int TOTAL_SPINS = 30;

    for (int spinNum = 1; spinNum <= TOTAL_SPINS; ++spinNum) {
        std::cout << "--- SPIN #" << spinNum << " ---" << std::endl;

        // Прокручиваем барабаны
        std::vector<std::vector<std::string>> symbols2;
        for (auto& reel : reels) {
            int steps = rng.generate(1, static_cast<int>(reel.size()));
            reel.spin(steps);
            symbols2.push_back(reel.getVisibleSymbols(reel.getPosition()));
        }

        // Показываем результат
        std::cout << "Result:" << std::endl;
        for (int row = 0; row < 3; ++row) {
            for (size_t reel = 0; reel < symbols2.size(); ++reel) {
                std::cout << std::setw(12) << symbols2[reel][row];
            }
            std::cout << std::endl;
        }

        // Проверяем линии
        int win2 = 0;
        bool hasWin = false;

        for (const auto& line : payTable->getLines()) {
            std::vector<std::string> lineSymbols;
            bool valid = true;

            for (size_t reelIdx = 0; reelIdx < line.positions.size(); ++reelIdx) {
                if (reelIdx >= symbols2.size()) {
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

                if (row >= static_cast<int>(symbols2[reelIdx].size())) {
                    valid = false;
                    break;
                }

                lineSymbols.push_back(symbols2[reelIdx][row]);
            }

            if (!valid || lineSymbols.empty()) {
                continue;
            }

            // 🔥 НОВАЯ ЛОГИКА: считаем ВСЕ одинаковые символы на линии (не только подряд)
            std::map<std::string, int> symbolCount;
            for (const auto& sym : lineSymbols) {
                symbolCount[sym]++;
            }

            // Проверяем каждый символ, который есть на линии
            for (const auto& [symbol, count] : symbolCount) {
                if (count >= 3) {
                    int payout = payTable->getPayout(symbol, count, betPerLine);
                    if (payout > 0) {
                        if (!hasWin) {
                            std::cout << "🏆 WINNING LINES:" << std::endl;
                            hasWin = true;
                        }
                        std::cout << "  Line " << line.id << " (" << line.name << "): ";
                        std::cout << count << "x " << symbol << " = " << payout << " coins" << std::endl;
                        win2 += payout;
                    }
                }
            }
        }

        if (!hasWin) {
            std::cout << "❌ No winning lines" << std::endl;
        }
        else {
            totalWins++;
            totalWinAmount += win2;
            if (win2 > maxWin) maxWin = win2;
        }

        balance -= totalBet;
        balance += win2;
        std::cout << "Win: " << win2 << " coins" << std::endl;
        std::cout << "Balance: " << balance << " coins" << std::endl;
        std::cout << std::endl;
        totalSpins++;

        if (balance < totalBet) {
            std::cout << "⚠️  Insufficient balance! Game over." << std::endl;
            break;
        }
    }

    // Выводим статистику
    std::cout << "========================================" << std::endl;
    std::cout << "  📊 STATISTICS" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "  Total spins:     " << totalSpins << std::endl;
    std::cout << "  Winning spins:   " << totalWins << std::endl;
    std::cout << "  Win rate:        " << std::fixed << std::setprecision(1)
        << (totalSpins > 0 ? (100.0 * totalWins / totalSpins) : 0.0) << "%" << std::endl;
    std::cout << "  Total winnings:  " << totalWinAmount << " coins" << std::endl;
    std::cout << "  Max win:         " << maxWin << " coins" << std::endl;
    std::cout << "  Final balance:   " << balance << " coins" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << std::endl << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return 0;
}