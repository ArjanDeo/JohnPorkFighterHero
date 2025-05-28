#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class Player {
public:
    Player(const std::string& texturePath, const std::string& name, int health);
    

    // Getters
    std::string getName() const;
    int getHealth() const;
    const std::string& getTexturePath() const;
    sf::Sprite& getSprite();

    // Setters
    void setSprite();
    void setHealth(int health);
    void takeDamage(int damage);
    bool isAlive() const;

private:
    std::string name;
    int health;
    std::string texturePath;
    sf::Texture texture;
    sf::Sprite sprite;
};
