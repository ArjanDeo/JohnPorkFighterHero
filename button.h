// button.h
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button
{
public:
    Button(const std::string& text);
    void draw(sf::RenderWindow& window);
    void setPosition(float x, float y);
    sf::RectangleShape buttonShape;

private:
    sf::Text buttonText;
    sf::Font font;
};
