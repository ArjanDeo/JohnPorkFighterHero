#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:

    // Constructor
    Button(const sf::Font &font, const std::string &label, unsigned int charSize = 48);

    // Methods
    bool isMouseOver(sf::RenderWindow &window) const;
    bool isClicked(sf::RenderWindow &window, const std::optional<sf::Event> &event);
    void draw(sf::RenderWindow &window);
    void setPosition(const sf::Vector2f& position);
	void setSize(const sf::Vector2f& size);
    void setFillColor(const sf::Color& color);
    std::string getText();

private:
    sf::Text text;
    sf::RectangleShape shape;
    void centerText();
};
