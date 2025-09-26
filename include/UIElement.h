#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class UIElement {
protected:
    int x, y;           // position
    int width, height;  // size
    bool visible;

public:
    UIElement(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height), visible(true) {
    }

    virtual ~UIElement() = default;

    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void update(float dt) {}

    void SetVisible(bool v) { visible = v; }
    bool IsVisible() const { return visible; }

    int GetX() const { return x; }
    int GetY() const { return y; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
};
