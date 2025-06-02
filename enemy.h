#pragma once
#include <SFML/Graphics.hpp>
#include "assetManager.h"

class Enemy {
public:
    Enemy(const std::string& texturePath, AssetManager& assetManager, sf::Vector2f scale);

    void spawn(const sf::Vector2f& position);
    void spawnAvoiding(const sf::FloatRect& avoidArea1, const sf::FloatRect& avoidArea2, const sf::Vector2u& windowSize);

    sf::Sprite& getSprite() { return sprite; }
    const sf::Sprite& getSprite() const { return sprite; }

    // Add more enemy-specific functionality as needed
    // void takeDamage(int amount);
    // bool isAlive() const;
    // etc.

private:
    sf::Sprite sprite;
    const sf::Texture& texture;
    sf::Vector2f scale;
};