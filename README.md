# 🎰 Burmalda - Детерминированная слот-машина

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.10%2B-brightgreen.svg)](https://cmake.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## 📖 О проекте

**Burmalda** — это детерминированная слот-машина с настоящим графическим интерфейсом (в разработке). Проект создан для портфолио с акцентом на чистую архитектуру, тестируемость и воспроизводимость.

### Особенности

- ✅ **Детерминированный RNG** — генератор с сохранением состояния
- ✅ **Гибкая конфигурация** — загрузка барабанов и выплат из JSON
- ✅ **Чистая архитектура** — отделение логики от GUI
- ✅ **5 барабанов × 3 ряда** — классическая механика
- ✅ **5 линий выплат** — центр, верх, низ, V-образная и перевёрнутая V
- ✅ **Консольная демонстрация** — 30 спинов со статистикой

## 🛠️ Технологии

- **Язык:** C++17
- **Сборка:** CMake 3.10+
- **JSON:** nlohmann/json (заголовочная библиотека)
- **GUI (планируется):** Qt6


## 🚀 Сборка и запуск

### Требования

- Компилятор с поддержкой C++17
- CMake 3.10+
- Git (для скачивания nlohmann/json)

### Инструкция

```bash
# Клонируем репозиторий
git clone https://github.com/ТВОЙ_НИК/Burmalda.git
cd Burmalda

# Создаём папку сборки
mkdir build && cd build

# Настраиваем CMake
cmake ..

# Собираем проект
cmake --build . --config Release

# Запускаем консольную демонстрацию
./Release/burmalda_console.exe
```

## 📊 Пример вывода
```bash
========================================
  🎰 BURMALDA SLOT MACHINE DEMO  🎰
========================================

Loading configuration...
✅ Config loaded!
   Lines: 5
   Symbols: 7

--- SPIN #1 ---
Result:
        Plum         Bar       Seven      Cherry  Watermelon
        Plum         Bar       Seven       Lemon         Bar
        Plum         Bar      Cherry       Lemon         Bar
❌ No winning lines

--- SPIN #4 ---
Result:
  Watermelon      Cherry         Bar      Orange  Watermelon
         Bar      Cherry         Bar      Orange         Bar
         Bar      Cherry         Bar  Watermelon         Bar
🏆 WINNING LINES:
  Line 1 (Center Line): 3x Bar = 100 coins
  Line 3 (Bottom Line): 3x Bar = 100 coins
  Line 5 (Inverted V): 3x Bar = 100 coins
Win: 300 coins

📊 STATISTICS
========================================
  Total spins:     30
  Winning spins:   8
  Win rate:        26.7%
  Total winnings:  1740 coins
  Max win:         600 coins
  Final balance:   1240 coins
========================================
```

## 🎯 Планы развития

- Детерминированный RNG
- Барабаны и линии выплат
- Загрузка конфигов из JSON
- Консольная демонстрация
- GUI на Qt6
- Анимация прокрутки
- Подсветка выигрышных линий
- Бонусная игра (фриспины)
- Аудит логгер
- Сохранение сессий
