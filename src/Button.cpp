#include "Button.h"

Button::Button(const sf::Font& font, const std::string& label, unsigned int charSize) : text(font), shape() {
    text.setString(label);
    text.setCharacterSize(charSize);
    text.setFillColor(sf::Color::White);

    shape.setFillColor(sf::Color(50, 50, 50, 200));
    shape.setSize({200.f, 60.f});
    centerText();

}


bool Button::isMouseOver(sf::RenderWindow &window) const {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return shape.getGlobalBounds().contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) });
}

bool Button::isClicked(sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    if (!event.has_value()) return false;
    
    if (const auto *mouseBtnPressed = event->getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseBtnPressed->button == sf::Mouse::Button::Left) {
            return isMouseOver(window);
        }
    }
    return false;
}

void Button::draw(sf::RenderWindow &window) {
    centerText();
    isMouseOver(window)
        ? setFillColor({ 100, 100, 100, 200 })
        : setFillColor({ 50, 50, 50, 200 });

    window.draw(shape);
    window.draw(text);
}

void Button::setPosition(const sf::Vector2f& position) {
	shape.setPosition(position);
}
void Button::setSize(const sf::Vector2f& size) {
	shape.setSize(size);
}
void Button::setFillColor(const sf::Color& color) {
	shape.setFillColor(color);
}
void Button::centerText()
{
    sf::FloatRect textBounds = text.getLocalBounds();

    text.setOrigin({
        textBounds.position.x + textBounds.size.x / 2.f,
        textBounds.position.y + textBounds.size.y / 2.f
        });
    text.setPosition({
        shape.getPosition().x + shape.getSize().x / 2.f,
        shape.getPosition().y + shape.getSize().y  / 2.f
        });
}
std::string Button::getText() {
	return text.getString();
}