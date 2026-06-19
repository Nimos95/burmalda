#pragma once
#include "paytable.h"
#include "reel.h"
#include <memory>
#include <vector>
#include <string>

class GameController {
public:
    GameController(std::shared_ptr<PayTable> payTable);

    void initGame();
    void spin(int betPerLine);

    std::vector<std::vector<std::string>> getCurrentSymbols() const;
    int getBalance() const { return m_balance; }
    int getLastWin() const { return m_lastWin; }
    std::vector<int> getWinningLines() const { return m_winningLines; }
    std::vector<int> getLinePayouts() const { return m_linePayouts; }
    const std::vector<PayLine>& getLines() const;

private:
    std::shared_ptr<PayTable> m_payTable;
    std::vector<Reel> m_reels;
    std::vector<std::vector<std::string>> m_currentSymbols;

    int m_balance = 1000;
    int m_lastWin = 0;
    int m_totalWins = 0;
    int m_totalSpins = 0;
    std::vector<int> m_winningLines;
    std::vector<int> m_linePayouts;
};