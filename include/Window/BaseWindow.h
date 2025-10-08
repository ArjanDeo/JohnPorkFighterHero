#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "GameState.h"

class BaseWindow {
public:
    // Protected constructor - only derived classes can call it
    virtual ~BaseWindow() = default;

    // Pure virtual methods (like C# interface methods)
    virtual void updateElementLayout() = 0;
    virtual void drawWindow() = 0;
    virtual void handleEvent(const std::optional<sf::Event>& event) = 0;

protected:
    // Protected constructor with member initialization
    BaseWindow(sf::RenderWindow& window, AssetManager& assetManager, GameState& gameState)
        : window(window), assetManager(assetManager), gameState(gameState) {
    }

    sf::RenderWindow& window;
    AssetManager& assetManager;
    GameState& gameState;
};