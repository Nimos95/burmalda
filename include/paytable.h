#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * Структура линии выплат
 */
struct PayLine {
    int id;
    std::vector<std::vector<int>> positions;  // [reel][row] -> row (0,1,2)
    std::string name;
};

/**
 * Таблица выплат - загружается из JSON
 */
class PayTable {
public:
    bool loadFromFile(const std::string& reelsFile, const std::string& paytableFile);

    // Выплата за комбинацию
    int getPayout(const std::string& symbol, int count, int betPerLine = 1) const;

    // Получить все линии
    const std::vector<PayLine>& getLines() const { return m_lines; }

    // Получить все символы
    const std::vector<std::string>& getSymbols() const { return m_symbols; }

    // Получить конфигурацию барабанов
    const std::vector<std::vector<std::string>>& getReelsConfig() const { return m_reelsConfig; }

private:
    std::vector<PayLine> m_lines;
    std::unordered_map<std::string, std::unordered_map<int, int>> m_payouts;
    std::vector<std::string> m_symbols;
    std::vector<std::vector<std::string>> m_reelsConfig;

    void parseReels(const json& j);
    void parsePaytable(const json& j);
    void parseLines(const json& j);
};