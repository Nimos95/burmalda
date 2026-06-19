#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "game_controller.h"
#include <memory>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Burmalda - Slot Machine");
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);

    auto payTable = std::make_shared<PayTable>();
    if (!payTable->loadFromFile("config/reels.json", "config/paytable.json")) {
        std::cerr << "Failed to load configs!" << std::endl;
        return 1;
    }

    GameController game(payTable);
    game.initGame();

    int betPerLine = 10;
    bool spinRequested = false;
    bool gameOver = false;
    sf::Clock deltaClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        // --- ОТРИСОВКА ФОНА ---
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(800, 600));
        ImGui::Begin("Background", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNav);

        // Рисуем зелёный фон
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddRectFilled(
            ImVec2(0, 0),
            ImVec2(800, 600),
            IM_COL32(26, 71, 42, 255)
        );

        // --- ЗАГОЛОВОК ---
        ImGui::SetCursorPosX(350);
        ImGui::SetCursorPosY(20);
        ImGui::Text("🎰 BURMALDA");

        // --- БАРАБАНЫ ---
        auto symbols = game.getCurrentSymbols();
        ImGui::SetCursorPosY(80);

        for (int row = 0; row < 3; ++row) {
            ImGui::SetCursorPosX(120);
            for (int reel = 0; reel < 5; ++reel) {
                std::string sym = symbols[reel][row];
                std::string display = sym;

                if (sym == "Cherry") display = "C";
                else if (sym == "Lemon") display = "L";
                else if (sym == "Plum") display = "P";
                else if (sym == "Orange") display = "O";
                else if (sym == "Watermelon") display = "W";
                else if (sym == "Bar") display = "B";
                else if (sym == "Seven") display = "7";

                // Цветные кнопки
                ImVec4 color;
                if (sym == "Cherry") color = ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
                else if (sym == "Lemon") color = ImVec4(0.8f, 0.8f, 0.1f, 1.0f);
                else if (sym == "Plum") color = ImVec4(0.5f, 0.1f, 0.5f, 1.0f);
                else if (sym == "Orange") color = ImVec4(0.9f, 0.5f, 0.1f, 1.0f);
                else if (sym == "Watermelon") color = ImVec4(0.2f, 0.8f, 0.2f, 1.0f);
                else if (sym == "Bar") color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
                else if (sym == "Seven") color = ImVec4(0.9f, 0.1f, 0.1f, 1.0f);
                else color = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

                ImGui::PushStyleColor(ImGuiCol_Button, color);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
                ImGui::Button(display.c_str(), ImVec2(80, 80));
                ImGui::PopStyleColor(3);
                ImGui::SameLine(0, 20);
            }
            ImGui::NewLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        }

        // --- КНОПКА SPIN ---
        ImGui::SetCursorPosX(350);
        ImGui::SetCursorPosY(420);

        // Проверяем, хватает ли баланса для игры
        int totalBet = betPerLine * payTable->getLines().size();
        bool canSpin = (game.getBalance() >= totalBet && !gameOver);

        // Блокируем кнопку, если не хватает денег
        if (!canSpin) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.1f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.3f, 0.2f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.1f, 0.0f, 1.0f));
        }

        if (ImGui::Button("SPIN", ImVec2(120, 50)) && canSpin) {
            spinRequested = true;
        }

        ImGui::PopStyleColor(3);

        // --- ИНФОРМАЦИЯ ---
        ImGui::SetCursorPosX(150);
        ImGui::SetCursorPosY(500);
        ImGui::Text("Баланс: %d coins", game.getBalance());

        ImGui::SameLine(0, 50);
        ImGui::Text("Последний выигрыш: %d coins", game.getLastWin());

        ImGui::SameLine(0, 50);
        ImGui::Text("Линий: %zu", payTable->getLines().size());

        // --- ВЫИГРЫШНЫЕ ЛИНИИ ---
        auto winningLines = game.getWinningLines();
        if (!winningLines.empty()) {
            ImGui::SetCursorPosX(150);
            ImGui::SetCursorPosY(540);
            std::string winText = "🏆 Выигрышные линии: ";
            for (int id : winningLines) {
                winText += std::to_string(id) + " ";
            }
            ImGui::Text("%s", winText.c_str());
        }
        else {
            ImGui::SetCursorPosX(150);
            ImGui::SetCursorPosY(540);
            ImGui::Text("Нет выигрышных линий");
        }

        // --- СООБЩЕНИЕ О КОНЦЕ ИГРЫ ---
        if (gameOver) {
            ImGui::SetCursorPosX(300);
            ImGui::SetCursorPosY(580);
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "ИГРА ОКОНЧЕНА!");
        }

        ImGui::End();

        // --- ОБРАБОТКА СПИНА ---
        if (spinRequested) {
            try {
                // Проверяем баланс перед спином
                int totalBet = betPerLine * payTable->getLines().size();
                if (game.getBalance() >= totalBet) {
                    game.spin(betPerLine);

                    // Проверяем, не закончились ли деньги
                    if (game.getBalance() < totalBet) {
                        gameOver = true;
                    }
                }
                else {
                    gameOver = true;
                }
            }
            catch (const std::exception& e) {
                std::cout << "Spin error: " << e.what() << std::endl;
                gameOver = true;
            }
            spinRequested = false;
        }

        // --- ОТРИСОВКА ---
        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}