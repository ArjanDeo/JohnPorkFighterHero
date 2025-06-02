#include "player.h"

Player::Player(const sf::Texture& texture, const std::string& name, int health, AssetManager& assetManager) 
    : texture(texture), name(name), health(health), assetManager(assetManager)
{
    health = 100;
	this->texture = texture;
    setSprite();
}

std::string Player::getName() const {
    return name;
}

int Player::getHealth() const {
    return health;
}

const sf::Texture& Player::getTexture() {
    return texture;
}
sf::Sprite& Player::getSprite() {
    return sprite;
}


void Player::setHealth(int newHealth) {
    health = (newHealth < 0) ? 0 : newHealth;
}

void Player::setSprite() {
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
