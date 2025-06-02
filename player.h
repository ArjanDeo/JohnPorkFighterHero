#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "assetManager.h"

class Player {
public:
    Player(const sf::Texture& texture, const std::string& name, int health, AssetManager& assetManager);   

    // Getters
    std::string getName() const;
    int getHealth() const;
    const sf::Texture& getTexture();
    sf::Sprite& getSprite();

    // Setters
    void setSprite();
    void setHealth(int health);
    void takeDamage(int damage);
    bool isAlive() const;

private:
	AssetManager& assetManager;
    std::string name;
    int health;
    std::string texturePath;
    sf::Texture texture;
    sf::Sprite sprite;
};
