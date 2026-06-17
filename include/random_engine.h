#pragma once

#include <random>
#include <fstream>
#include <string>
#include <cstdint>

/**
 * Класс RandomEngine - детерминированный генератор случайных чисел
 *
 * Использует Mersenne Twister (std::mt19937) для качественной генерации
 * Поддерживает сохранение и восстановление состояния
 */
class RandomEngine {
public:
    // Конструктор с seed по умолчанию
    explicit RandomEngine(uint64_t seed = 5489u);

    // Генерация числа в диапазоне [min, max] (включительно)
    int generate(int min, int max);

    // Генерация числа в диапазоне [0.0, 1.0) (для вероятностей)
    double generateDouble();

    // Сохранить состояние генератора в файл
    void saveState(const std::string& filename) const;

    // Восстановить состояние генератора из файла
    void loadState(const std::string& filename);

    // Получить seed (для отладки)
    uint64_t getSeed() const { return m_seed; }

private:
    uint64_t m_seed;          // Начальное число
    std::mt19937 m_rng;       // Генератор Mersenne Twister
    std::uniform_int_distribution<int> m_intDist;      // Для целых чисел
    std::uniform_real_distribution<double> m_realDist; // Для дробных чисел
};