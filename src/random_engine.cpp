#include "random_engine.h"
#include <stdexcept>   // Для std::runtime_error
#include <sstream>     // Для строковых потоков

// -----------------------------------------------------------------------------
// Конструктор
// -----------------------------------------------------------------------------
RandomEngine::RandomEngine(uint64_t seed)
    : m_seed(seed)                                    // Сохраняем seed
    , m_rng(static_cast<std::mt19937::result_type>(seed))  // Инициализируем генератор
    , m_intDist(0, 1)      // Временный диапазон (будет меняться в generate)
    , m_realDist(0.0, 1.0) // Диапазон [0, 1) для double
{
    // Тело конструктора пустое - всё сделано в списке инициализации
}

// -----------------------------------------------------------------------------
// Генерация целого числа
// -----------------------------------------------------------------------------
int RandomEngine::generate(int min, int max) {
    // Если min > max, меняем местами (защита от дурака)
    if (min > max) {
        std::swap(min, max);
    }

    // Создаём параметр с нужным диапазоном и передаём генератору
    using Param = std::uniform_int_distribution<int>::param_type;
    return m_intDist(m_rng, Param(min, max));
}

// -----------------------------------------------------------------------------
// Генерация дробного числа
// -----------------------------------------------------------------------------
double RandomEngine::generateDouble() {
    return m_realDist(m_rng);
}

// -----------------------------------------------------------------------------
// Сохранение состояния в файл
// -----------------------------------------------------------------------------
void RandomEngine::saveState(const std::string& filename) const {
    // Открываем файл для бинарной записи
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) {
        throw std::runtime_error("Cannot save RNG state to " + filename);
    }

    // Сериализуем генератор в строку
    std::ostringstream oss;
    oss << m_rng;
    std::string stateStr = oss.str();

    // Записываем: сначала размер строки, потом саму строку
    size_t size = stateStr.size();
    ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));
    ofs.write(stateStr.data(), size);

    // Записываем seed
    ofs.write(reinterpret_cast<const char*>(&m_seed), sizeof(m_seed));
}

// -----------------------------------------------------------------------------
// Восстановление состояния из файла
// -----------------------------------------------------------------------------
void RandomEngine::loadState(const std::string& filename) {
    // Открываем файл для бинарного чтения
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) {
        throw std::runtime_error("Cannot load RNG state from " + filename);
    }

    // Читаем размер строки
    size_t size;
    ifs.read(reinterpret_cast<char*>(&size), sizeof(size));

    // Читаем строку состояния
    std::string stateStr(size, '\0');
    ifs.read(&stateStr[0], size);

    // Читаем seed
    ifs.read(reinterpret_cast<char*>(&m_seed), sizeof(m_seed));

    // Восстанавливаем генератор из строки
    std::istringstream iss(stateStr);
    iss >> m_rng;
}