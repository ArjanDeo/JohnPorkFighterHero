#pragma once
#include "UIElement.h"
#include <SFML/Graphics.hpp>
#include <string>

class Button : public UIElement {
public:
    Button(const sf::Font& font,
        const std::string& label,
        unsigned int charSize = 24);

    void setPosition(const sf::Vector2f& pos);
    void setSize(const sf::Vector2f& size);
    void setFillColor(const sf::Color& color);
    void setHoverColor(const sf::Color& color);

    std::string getText() const { return text.getString(); }

    // UIElement implementation
    void handleEvent(const std::optional<sf::Event>& event) override;
    void update(float dt) override {}
    void draw(sf::RenderWindow& window) override;

private:
    void centerText();

    sf::Text text;
    sf::RectangleShape shape;
    sf::Color normalColor;
    sf::Color hoverColor;
    bool isHovered = false;
};
