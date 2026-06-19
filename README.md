# 🎰 Burmalda - Детерминированная слот-машина

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.20%2B-brightgreen.svg)](https://cmake.org/)
[![SFML](https://img.shields.io/badge/SFML-2.5%2B-orange.svg)](https://www.sfml-dev.org/)
[![ImGui](https://img.shields.io/badge/ImGui-1.90-red.svg)](https://github.com/ocornut/imgui)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## 📖 О проекте

**Burmalda** — это детерминированная слот-машина с графическим интерфейсом на **SFML + ImGui**. Проект создан для портфолио с акцентом на чистую архитектуру, тестируемость и воспроизводимость.

### Особенности

- ✅ **Детерминированный RNG** — генератор с сохранением состояния
- ✅ **Гибкая конфигурация** — загрузка барабанов и выплат из JSON
- ✅ **Чистая архитектура** — отделение логики от GUI (GameController)
- ✅ **5 барабанов × 3 ряда** — классическая механика
- ✅ **5 линий выплат** — центр, верх, низ, V-образная и перевёрнутая V
- ✅ **Консольная демонстрация** — 30 спинов со статистикой
- ✅ **Графический интерфейс на SFML + ImGui** — кнопки, барабаны, баланс
- ✅ **Поддержка русского языка** — загрузка шрифта с кириллицей
- ✅ **Цветные символы** — визуальное отображение барабанов
- ✅ **Автоматическая загрузка JSON** — через CMake FetchContent

## 🛠️ Технологии

- **Язык:** C++17
- **Сборка:** CMake 3.20+
- **Графика:** SFML 2.5+
- **GUI:** ImGui 1.90
- **JSON:** nlohmann/json (автозагрузка через CMake)
- **Шрифты:** Поддержка кириллицы через Arial.ttf


## 🚀 Сборка и запуск

### Требования

- Компилятор с поддержкой C++17 (MSVC 2022, GCC, Clang)
- CMake 3.20+
- Git (для автоматической загрузки nlohmann/json)

### Инструкция для Visual Studio 2022

```bash
# Клонируем репозиторий
git clone https://github.com/ТВОЙ_НИК/Burmalda.git
cd Burmalda

# Открываем папку в Visual Studio 2022
# VS автоматически настроит CMake

# Выбираем запускаемый проект:
# - burmalda_console  → консольная версия
# - burmalda_gui      → GUI версия

# Сборка: Ctrl+Shift+B
# Запуск: F5
```

### Инструкция для cmd

```bash
# Клонируем репозиторий
git clone https://github.com/ТВОЙ_НИК/Burmalda.git
cd Burmalda

# Создаём папку сборки
mkdir build && cd build

# Настраиваем CMake (автоматически скачает nlohmann/json)
cmake .. -G "Visual Studio 17 2022" -A x64

# Собираем проект
cmake --build . --config Release

# Запускаем консольную демонстрацию
./Release/burmalda_console.exe

# Запускаем GUI версию
./Release/burmalda_gui.exe
```

## 📊 Пример вывода (консоль)
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

- Цветные символы барабанов
- Логика баланса и выигрышей
- Анимация прокрутки барабанов
- Подсветка выигрышных линий
- Картинки вместо букв
- Бонусная игра (фриспины)
- Аудит логгер
- Сохранение сессий
- Звуки и музыка
