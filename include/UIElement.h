#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include <optional>

class UIElement {
protected:
    int x, y;           // position
    int width, height;  // size
    bool visible;
    std::function<void()> onClick;

public:
    UIElement(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height), visible(true) {
    }

    virtual ~UIElement() = default;

    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void update(float dt) {}

    // onClick functionality
    virtual void handleEvent(const std::optional<sf::Event>& event) {
        if (!event.has_value() || !visible) return;

        if (const auto* mouseBtnPressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseBtnPressed->button != sf::Mouse::Button::Left) return;

            sf::Vector2f mousePos(static_cast<float>(mouseBtnPressed->position.x),
                static_cast<float>(mouseBtnPressed->position.y));

            if (isPointInside(mousePos.x, mousePos.y) && onClick) {
                onClick();
            }
        }
    }

    void setOnClick(std::function<void()> callback) {
        onClick = callback;
    }

    // Check if a point is inside the element's bounds
    virtual bool isPointInside(int pointX, int pointY) const {
        return pointX >= x && pointX <= x + width &&
            pointY >= y && pointY <= y + height;
    }

    // Position and size management
    virtual void setPosition(const sf::Vector2f& pos) {
        x = static_cast<int>(pos.x);
        y = static_cast<int>(pos.y);
    }

    virtual void setSize(const sf::Vector2f& size) {
        width = static_cast<int>(size.x);
        height = static_cast<int>(size.y);
    }

    void SetVisible(bool v) { visible = v; }
    bool IsVisible() const { return visible; }
    int GetX() const { return x; }
    int GetY() const { return y; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
};