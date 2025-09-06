#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "Button.h"
#include "GameState.h"
#include "MainMenu.h"
#include "SettingsMenu.h"


static sf::Image GameIcon(AssetManager &assetManager) {
    sf::Texture &iconTexture = assetManager.getTexture("gameIcon.png");
    
    sf::Image iconImage = iconTexture.copyToImage();
    
    return iconImage;
}

static sf::RenderWindow GameWindow(AssetManager& assetManager) {
    sf::RenderWindow window;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::Image gameIcon = GameIcon(assetManager);

    window.create(desktop, "John Pork: Fighter Hero");
    window.setIcon(gameIcon.getSize(), gameIcon.getPixelsPtr());
    return window;
}

int main() {
    AssetManager assetManager("assets");
    sf::Font& font = assetManager.getFont("Raleway.ttf");
    sf::Music currentMusic;
    GameState gameState;

	gameState = GameState::MAIN_MENU;

    sf::RenderWindow window = GameWindow(assetManager);
    const sf::Vector2u MIN_SIZE(1200, 720);
	window.setMinimumSize(MIN_SIZE);
    MainMenu mainMenu(window, assetManager, gameState);
    SettingsMenu settingsMenu(window, assetManager, gameState);
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* windowResized = event->getIf<sf::Event::Resized>()) {
                sf::FloatRect visibleArea(
                    sf::Vector2f(0.f, 0.f),
                    sf::Vector2f(windowResized->size.x, windowResized->size.y)
                );
                window.setView(sf::View(visibleArea));
            }

            if (gameState == GameState::MAIN_MENU)
                mainMenu.handleEvents(event);
            else if (gameState == GameState::SETTINGS_MENU)
                settingsMenu.handleEvents(event);
        }

        window.clear();

        switch (gameState)
        {
        case GameState::MAIN_MENU:
            mainMenu.DrawMainMenu();
            break;
        case GameState::SETTINGS_MENU:
            settingsMenu.DrawSettingsMenu();
            break;
        default:
            break;
        }
        
        window.display();
    }
    return 0;
}