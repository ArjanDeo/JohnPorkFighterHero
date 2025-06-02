#include "button.h"

Button::Button(const std::string& text, const float x, const float y, const sf::Font& font) : x(x), y(y)
{
    buttonShape.setSize({ 125.f, 62.5f });
    buttonShape.setFillColor(sf::Color::Blue);
    setPosition();

    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(20);
    buttonText.setFillColor(sf::Color::White);

    auto textBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(textBounds.left + textBounds.width / 2.f,
        textBounds.top + textBounds.height / 2.f);

}

void Button::setPosition()
{
    buttonShape.setPosition(x, y);
    buttonText.setPosition(
        x + buttonShape.getSize().x / 2.f,
        y + buttonShape.getSize().y / 2.f
    );
}

void Button::draw(sf::RenderWindow& window)
{
    window.draw(buttonShape);
    window.draw(buttonText);
}
const sf::RectangleShape& Button::getShape() const {
	return buttonShape;
}
void Button::setText(const std::string& text) {
	buttonText.setString(text);
	auto textBounds = buttonText.getLocalBounds();
	buttonText.setOrigin(textBounds.left + textBounds.width / 2.f,
		textBounds.top + textBounds.height / 2.f);
}