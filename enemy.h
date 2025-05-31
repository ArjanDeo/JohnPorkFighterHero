#pragma once
#include <SFML/Graphics.hpp>
#include <random>
class Enemy
{
	public:
		Enemy(const std::string& texturePath, sf::Vector2f scale = { 0.45f, 0.45f });
		sf::Sprite& getSprite();
	private:
		void setSprite();
		sf::Vector2f setPosition();
		sf::Sprite sprite;
		sf::Vector2f position;
		sf::Vector2f scale;
		sf::Texture enemyTexture;
		std::string texturePath;
};