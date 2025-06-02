// enemy.cpp
#include "enemy.h"
#include <random>

Enemy::Enemy(const std::string& texturePath, AssetManager& assetManager, sf::Vector2f scale)
    : texture(assetManager.getTexture(texturePath)), scale(scale)
{
    sprite.setTexture(texture);
    sprite.setScale(scale);
}

void Enemy::spawn(const sf::Vector2f& position) {
    sprite.setPosition(position);
}

void Enemy::spawnAvoiding(const sf::FloatRect& avoidArea1, const sf::FloatRect& avoidArea2, const sf::Vector2u& windowSize) {
    const float marginX = windowSize.x * 0.2f;
    const float marginY = windowSize.y * 0.2f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(0.0f, windowSize.x - marginX);
    std::uniform_real_distribution<float> distY(0.0f, windowSize.y - marginY);

    sf::Vector2f spawnPos;
    sf::FloatRect enemyBounds;

    do {
        spawnPos = { distX(gen), distY(gen) };
        sprite.setPosition(spawnPos);
        enemyBounds = sprite.getGlobalBounds();
    } while (enemyBounds.intersects(avoidArea1) || enemyBounds.intersects(avoidArea2));
}