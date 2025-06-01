// button.h
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button
{
public:
    Button(const std::string& text, const float x, const float y);
    void draw(sf::RenderWindow& window);
    void setPosition();
    const sf::RectangleShape& getShape() const;
private:
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
    sf::Font font;
    const float x;
	const float y;
};
