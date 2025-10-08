#include "SettingsMenu.h"
#include <Common.h>

SettingsMenu::SettingsMenu(sf::RenderWindow& window, AssetManager& assetManager, GameState& gameState)
	: gameState(gameState), window(window), assetManager(assetManager), font(assetManager.getFont("Raleway.ttf")), BaseWindow(window, assetManager, gameState)
{
    windowSize = window.getSize();
    if (settingsButtons.empty())
        InitializeButtons();
}

void SettingsMenu::InitializeButtons() {
    float startX = windowSize.x * 0.1f;
    float startY = windowSize.y * 0.2f;
    float spacing = 80;

    for (size_t i = 0; i < labels.size(); ++i) {
        Button btn(font, labels[i]);
        btn.setPosition(sf::Vector2f(startX, startY + i * spacing));
		bool isFullscreen = false;
        if (labels[i] == "Close")
            btn.setOnClick([this]() { gameState = GameState::MAIN_MENU; });
        else if (labels[i] == "Vsync")
            btn.setOnClick([this]() { window.setVerticalSyncEnabled(true); });
        else if (labels[i] == "Fullscreen")
            btn.setOnClick([this]() { Common cmn; cmn.ToggleFullscreen(window); });
        settingsButtons.push_back(btn);
    }
}

void SettingsMenu::handleEvent(const std::optional<sf::Event>& event) {
    for (auto& button : settingsButtons) {
        button.handleEvent(event);
    }
}

void SettingsMenu::drawWindow() {
    sf::RectangleShape background;
    background.setFillColor(sf::Color(0, 0, 0, 200));
    background.setSize(sf::Vector2f(windowSize.x, windowSize.y));

    window.draw(background);

    for (auto& button : settingsButtons)
        button.draw(window);
}

void SettingsMenu::updateElementLayout() {
    float startX = windowSize.x * 0.1f;
    float startY = windowSize.y * 0.2f;
    float spacing = 80;
    float buttonWidth = windowSize.x * 0.8f;
    float buttonHeight = 60;
    for (size_t i = 0; i < settingsButtons.size(); ++i) {
        settingsButtons[i].setSize({ buttonWidth, buttonHeight });
        settingsButtons[i].setPosition({ startX, startY + i * spacing });
    }
}