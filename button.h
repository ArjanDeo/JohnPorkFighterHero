// button.h
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button
{
public:
    Button(const std::string& text, const float x, const float y, const sf::Font& font);
    void draw(sf::RenderWindow& window);
    void setPosition();
    void setText(const std::string& text);
    const sf::RectangleShape& getShape() const;
private:
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
    sf::Font font;
    const float x;
	const float y;
};
