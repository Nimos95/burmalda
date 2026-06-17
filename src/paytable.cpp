#include "paytable.h"
#include <fstream>
#include <iostream>
#include <algorithm>

bool PayTable::loadFromFile(const std::string& reelsFile, const std::string& paytableFile) {
    try {
        std::ifstream reelsStream(reelsFile);
        if (!reelsStream) {
            std::cerr << "Cannot open: " << reelsFile << std::endl;
            return false;
        }
        json reelsJson = json::parse(reelsStream);

        std::ifstream payStream(paytableFile);
        if (!payStream) {
            std::cerr << "Cannot open: " << paytableFile << std::endl;
            return false;
        }
        json payJson = json::parse(payStream);

        parseReels(reelsJson);
        parsePaytable(payJson);
        parseLines(payJson);

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading configs: " << e.what() << std::endl;
        return false;
    }
}

void PayTable::parseReels(const json& j) {
    m_reelsConfig.clear();
    for (const auto& reelArray : j["reels"]) {
        std::vector<std::string> reel;
        for (const auto& symbol : reelArray) {
            reel.push_back(symbol.get<std::string>());
        }
        m_reelsConfig.push_back(reel);
    }

    // Собираем уникальные символы
    m_symbols.clear();
    for (const auto& reel : m_reelsConfig) {
        for (const auto& sym : reel) {
            if (std::find(m_symbols.begin(), m_symbols.end(), sym) == m_symbols.end()) {
                m_symbols.push_back(sym);
            }
        }
    }
}

void PayTable::parsePaytable(const json& j) {
    m_payouts.clear();
    for (const auto& item : j["payouts"]) {
        std::string symbol = item["symbol"];
        for (const auto& pay : item["payments"]) {
            int count = pay["count"];
            int payout = pay["payout"];
            m_payouts[symbol][count] = payout;
        }
    }
}

void PayTable::parseLines(const json& j) {
    m_lines.clear();
    for (const auto& lineJson : j["lines"]) {
        PayLine line;
        line.id = lineJson["id"];
        line.name = lineJson.value("name", "Line " + std::to_string(line.id));

        // Очищаем positions перед заполнением
        line.positions.clear();

        // Проходим по каждому барабану в линии
        for (const auto& pos : lineJson["positions"]) {
            std::vector<int> reelPos;
            // pos - это массив с одной позицией для каждого барабана
            for (const auto& row : pos) {
                reelPos.push_back(row.get<int>());
            }
            line.positions.push_back(reelPos);
        }
        m_lines.push_back(line);
    }
}

int PayTable::getPayout(const std::string& symbol, int count, int betPerLine) const {
    auto symIt = m_payouts.find(symbol);
    if (symIt == m_payouts.end()) return 0;

    auto countIt = symIt->second.find(count);
    if (countIt == symIt->second.end()) return 0;

    return countIt->second * betPerLine;
}