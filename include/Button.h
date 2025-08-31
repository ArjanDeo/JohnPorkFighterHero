#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    sf::RectangleShape shape;
    sf::Text text;
    bool clicked = false;

    // Constructor
    Button(const sf::Font &font, const std::string &label, unsigned int charSize = 36);

    // Methods
    bool isMouseOver(sf::RenderWindow &window) const;
    bool isClicked(sf::RenderWindow &window, const std::optional<sf::Event> &event);
    void draw(sf::RenderWindow &window) const;
};
