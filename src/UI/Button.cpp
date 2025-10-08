#include "UI/Button.h"

Button::Button(const sf::Font& font,
    const std::string& label,
    unsigned int charSize) : UIElement(0, 0, 150, 50), text(font) {
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(charSize);
    text.setFillColor(sf::Color::White);
    shape.setSize({ 150.f, 50.f });
    normalColor = sf::Color(82, 86, 92);
    hoverColor = sf::Color(18, 74, 138);
    shape.setFillColor(normalColor);
    centerText();
}

void Button::setFillColor(const sf::Color& color) {
    normalColor = color;
    shape.setFillColor(color);
}

void Button::setHoverColor(const sf::Color& color) {
    hoverColor = color;
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

void Button::centerText() {
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({
        textBounds.position.x + textBounds.size.x / 2.f,
        textBounds.position.y + textBounds.size.y / 2.f
        });
    text.setPosition({
        shape.getPosition().x + shape.getSize().x / 2.f,
        shape.getPosition().y + shape.getSize().y / 2.f
        });
}
void Button::setPosition(const sf::Vector2f& pos) {
    UIElement::setPosition(pos);  // Update base class position
    shape.setPosition(pos);
    centerText();
}

void Button::setSize(const sf::Vector2f& size) {
    UIElement::setSize(size);  // Update base class size
    shape.setSize(size);
    centerText();
}



void Button::handleEvent(const std::optional<sf::Event>& event) {
    if (!event.has_value() || !visible) return;

    if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f mousePos(static_cast<float>(mouseMoved->position.x),
            static_cast<float>(mouseMoved->position.y));
            bool wasHovered = isHovered;
            isHovered = isPointInside(mousePos.x, mousePos.y);

            if (isHovered != wasHovered) {
                shape.setFillColor(isHovered ? hoverColor : normalColor);
            }
    }

    // Call parent to handle click logic
    UIElement::handleEvent(event);
}