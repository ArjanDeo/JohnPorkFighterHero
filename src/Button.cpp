#include "Button.h"

Button::Button(const sf::Font &font, const std::string &label, unsigned int charSize) : text(font), shape(), clicked(false) {
    text.setString(label);
    text.setCharacterSize(charSize);
    text.setFillColor(sf::Color::White);

    shape.setFillColor(sf::Color(50, 50, 50, 200));
    shape.setSize({200.f, 60.f});
}

bool Button::isMouseOver(sf::RenderWindow &window) const {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return shape.getGlobalBounds().contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) });
}

bool Button::isClicked(sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    if (!event.has_value()) return false;
    
    if (const auto *mouseBtnPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseBtnPressed->button == sf::Mouse::Button::Left) {
            if (isMouseOver(window)) {
                clicked = true;
                return true;
            }
        }
    }
    return false;
}



void Button::draw(sf::RenderWindow &window) const {
    window.draw(shape);
    window.draw(text);
}
