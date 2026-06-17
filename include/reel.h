#pragma once
#include <vector>
#include <string>

/**
 * Класс Reel - барабан игрового автомата
 *
 * Хранит последовательность символов и текущую позицию
 */
class Reel {
public:
    // Конструктор: принимает вектор символов
    explicit Reel(const std::vector<std::string>& symbols);

    // Получить символ по позиции (с циклическим перебором)
    std::string getSymbol(int position) const;

    // Получить 3 видимых символа: [верхний, центральный, нижний]
    std::vector<std::string> getVisibleSymbols(int currentPosition) const;

    // Прокрутить барабан на steps позиций
    int spin(int steps);

    // Текущая позиция (индекс центрального символа)
    int getPosition() const { return m_currentPosition; }

    // Количество символов на барабане
    size_t size() const { return m_symbols.size(); }

private:
    std::vector<std::string> m_symbols;
    int m_currentPosition;  // индекс центрального символа (0-based)
};