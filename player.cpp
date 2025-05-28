#include "player.h"
#include <SFML/Graphics.hpp>

Player::Player(const std::string& texturePath, const std::string& name, int health) : texturePath(texturePath), name(name), health(health)
{
    health = 100;
    setSprite();
}

std::string Player::getName() const {
    return name;
}

int Player::getHealth() const {
    return health;
}

const std::string& Player::getTexturePath() const {
    return texturePath;
}
sf::Sprite& Player::getSprite() {
    return sprite;
}


void Player::setHealth(int newHealth) {
    health = (newHealth < 0) ? 0 : newHealth;
}

void Player::setSprite() {
    if (!texture.loadFromFile(texturePath)) {
        throw std::runtime_error("Failed to load texture from " + texturePath);
    }
    sprite.setTexture(texture);
    sprite.setPosition(100.f, 100.f);
	sprite.setScale(0.85f, 0.85f);
}
void Player::takeDamage(int amount) {
    setHealth(health - amount);
}

bool Player::isAlive() const {
    return health > 0;
}
