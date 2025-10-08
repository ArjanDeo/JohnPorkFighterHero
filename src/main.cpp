#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "UI/Button.h"
#include "GameState.h"
#include "MainMenu.h"
#include "SettingsMenu.h"
#include "MissionMenu.h"
#include "StackManager.h"
#include <memory>

// Load the game icon from assets and convert it to an SFML Image.
// Used for setting the window icon.
static sf::Image GameIcon(AssetManager& assetManager) {
    sf::Texture& iconTexture = assetManager.getTexture("gameIcon.png");
    sf::Image iconImage = iconTexture.copyToImage();
    return iconImage;
}

// Initialize the main SFML RenderWindow with desktop resolution, minimum size, and icon.
static sf::RenderWindow GameWindow(AssetManager& assetManager) {
    const sf::Vector2u MIN_SIZE(1200, 800);
    sf::RenderWindow window;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::Image gameIcon = GameIcon(assetManager);

    window.create(desktop, "John Pork: Fighter Hero");
    window.setIcon(gameIcon);
    window.setMinimumSize(MIN_SIZE);
    return window;
}

int main() {
    // AssetManager handles fonts, textures, and other resources
    AssetManager assetManager("assets");
    sf::Font& font = assetManager.getFont("Raleway.ttf"); // font reference for menus/buttons
    sf::Music currentMusic;                               // currently unused, reserved for future music
    GameState gameState;

    gameState = GameState::MAIN_MENU;  // initial game state

    sf::RenderWindow window = GameWindow(assetManager);

    // StackManager holds all active windows (menus/screens)
    // Windows are pushed in reverse order here so MainMenu is on top initially
    StackManager stack;
    stack.pushWindow(std::make_unique<SettingsMenu>(window, assetManager, gameState));
    stack.pushWindow(std::make_unique<MainMenu>(window, assetManager, gameState));

    // Keep track of the previous state to detect when a transition is needed
    GameState previousState = gameState;

    while (window.isOpen())
    {
        // Check if the game state has changed (e.g., player clicked "Settings" button)
        if (gameState != previousState)
        {
            switch (gameState)
            {
            case GameState::MAIN_MENU:
                // Pop everything above MainMenu to return to it
                while (stack.top() && dynamic_cast<MainMenu*>(stack.top()) == nullptr)
                    stack.popWindow();

                // If somehow the stack is empty, push a new MainMenu
                if (!stack.top())
                    stack.pushWindow(std::make_unique<MainMenu>(window, assetManager, gameState));
                break;

            case GameState::SETTINGS_MENU:
                // Push a SettingsMenu if it's not already the top window
                if (!stack.top() || dynamic_cast<SettingsMenu*>(stack.top()) == nullptr)
                    stack.pushWindow(std::make_unique<SettingsMenu>(window, assetManager, gameState));
                break;

            case GameState::MISSION_MENU:
                // Push the mission screen if it's not already the top window
                if (!stack.top() || dynamic_cast<MissionMenu*>(stack.top()) == nullptr)
                    stack.pushWindow(std::make_unique<MissionMenu>(window, assetManager, gameState));
                break;

            default:
                break;
            }
            previousState = gameState; // update the previous state
        }

        // Event loop: handle all pending SFML events
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            // Handle window resize dynamically so content scales correctly
            if (const auto* windowResized = event->getIf<sf::Event::Resized>())
                window.setView(sf::View(sf::Vector2f(0.f, 0.f),
                    sf::Vector2f(windowResized->size.x, windowResized->size.y)));

            // Forward events to the current active window (top of stack)
            if (stack.top())
                stack.top()->handleEvent(event);
        }

        window.clear();

        // Draw the top window on the stack (only one active window is rendered)
        if (stack.top())
            stack.top()->drawWindow();

        window.display();
    }
    return 0;
}
