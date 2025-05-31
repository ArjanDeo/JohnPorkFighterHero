#include "enemy.h"

Enemy::Enemy(const std::string& texturePath, sf::Vector2f scale) : texturePath(texturePath), scale(scale)
{
	this->position = setPosition();
	setSprite();
}

sf::Vector2f Enemy::setPosition() {
	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> xrange(0, desktopMode.width);
	std::uniform_int_distribution<> yrange(0, desktopMode.height);
	float x = static_cast<float>(xrange(gen));
	float y = static_cast<float>(yrange(gen));
	return sf::Vector2f(x, y);
}

sf::Sprite& Enemy::getSprite() {
	return sprite;
}
void Enemy::setSprite() {
    static sf::Texture sharedTexture;
    static bool textureLoaded = false;

    if (!textureLoaded) {
        if (!sharedTexture.loadFromFile(texturePath)) {
            throw std::runtime_error("Failed to load enemy texture from " + texturePath);
        }
        textureLoaded = true;
    }

    sprite.setTexture(sharedTexture);
    sprite.setScale(scale);
    sprite.setPosition(position);
}
