#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "Button.h"
#include "GameState.h"
#include "MainMenu.h"
AssetManager assetManager("assets");
sf::Font& font = assetManager.getFont("Raleway.ttf");
sf::Music currentMusic;

sf::Image GameIcon() {
    sf::Texture &iconTexture = assetManager.getTexture("gameIcon.png");
    
    sf::Image iconImage = iconTexture.copyToImage();
    
    return iconImage;
}

sf::RenderWindow GameWindow() {
    sf::RenderWindow window;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::Image gameIcon = GameIcon();

    window.create(desktop, "John Pork: Fighter Hero");
    window.setIcon(gameIcon.getSize(), gameIcon.getPixelsPtr());
    return window;
}



int main() {
    GameState gameState = GameState::MAIN_MENU;

    sf::RenderWindow window = GameWindow();
    const sf::Vector2u MIN_SIZE(1200, 720);
	window.setMinimumSize(MIN_SIZE);

    MainMenu mainMenu(window, assetManager);

    while (window.isOpen())
    {
        window.clear();
        switch (gameState)
        {
        case GameState::MAIN_MENU:
            mainMenu.DrawMainMenu();
            break;
        
        default:
            break;
        }
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto *windowResized = event->getIf<sf::Event::Resized>()) {
                sf::FloatRect visibleArea(
                    sf::Vector2f(0.f, 0.f),
                    sf::Vector2f(windowResized->size.x, windowResized->size.y)
                );
                window.setView(sf::View(visibleArea));
            }

			if (gameState == GameState::MAIN_MENU)
				mainMenu.handleEvents(event);
        }
        window.display();
    }
    return 0;
}