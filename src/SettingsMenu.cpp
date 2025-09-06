#include "SettingsMenu.h"
#include <Common.h>

SettingsMenu::SettingsMenu(sf::RenderWindow& window, AssetManager& assetManager, GameState& gameState) 
	: gameState(gameState), window(window), assetManager(assetManager), font(assetManager.getFont("Raleway.ttf"))
{
    windowSize = window.getSize();
}

void SettingsMenu::InitializeButtons() {

	float startX = windowSize.x * 0.1f;
	float startY = windowSize.y * 0.2f;
	float spacing = 80;
	float buttonWidth = windowSize.x * 0.2f;
	float buttonHeight = 60;

    for (size_t i = 0; i < labels.size(); ++i) {
        Button btn(font, labels[i]);
        btn.setSize(sf::Vector2f(buttonWidth, buttonHeight));
        btn.setFillColor(sf::Color(50, 50, 50, 200));
        btn.setPosition(sf::Vector2f(startX, startY + i * spacing));
       
        settingsButtons.push_back(btn);
	}

}

void SettingsMenu::DrawSettingsMenu() {
    Common common;

    sf::Vector2u winSize = window.getSize();

    sf::RectangleShape background;
    sf::Color backgroundColor(0, 0, 0, 200);

    background.setFillColor(backgroundColor);
    background.setPosition(sf::Vector2f(0.f, 0.f));
    background.setSize(sf::Vector2f(static_cast<float>(winSize.x), static_cast<float>(winSize.y)));
	if (settingsButtons.empty())
        InitializeButtons();
    window.draw(background);

	for (auto& button : settingsButtons) {
		button.draw(window);
	}
}
void SettingsMenu::handleEvents(const std::optional<sf::Event>& event) {
    if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonReleased>()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        for (auto& button : settingsButtons) {
            if (button.isClicked(window, event)) {
                if (button.getText() == "Close") {
                    gameState = GameState::MAIN_MENU;
                }
                else if (button.getText() == "Vsync") {
                    window.setVerticalSyncEnabled(true);
                }
            }
        }
	}
}
