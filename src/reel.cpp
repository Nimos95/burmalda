#include "reel.h"
#include <stdexcept>

Reel::Reel(const std::vector<std::string>& symbols)
    : m_symbols(symbols)
    , m_currentPosition(0)
{
    if (m_symbols.empty()) {
        throw std::runtime_error("Reel cannot be empty");
    }
}

std::string Reel::getSymbol(int position) const {
    // Нормализуем позицию (поддерживаем отрицательные индексы)
    int normalized = position % static_cast<int>(m_symbols.size());
    if (normalized < 0) {
        normalized += static_cast<int>(m_symbols.size());
    }
    return m_symbols[normalized];
}

std::vector<std::string> Reel::getVisibleSymbols(int currentPosition) const {
    return {
        getSymbol(currentPosition - 1),  // верхний
        getSymbol(currentPosition),      // центральный
        getSymbol(currentPosition + 1)   // нижний
    };
}

int Reel::spin(int steps) {
    steps = steps % static_cast<int>(m_symbols.size());
    m_currentPosition = (m_currentPosition + steps) % static_cast<int>(m_symbols.size());
    return m_currentPosition;
}