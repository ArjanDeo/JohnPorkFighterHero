#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

sf::Image GameIcon() {
    sf::Image icon;
    icon.loadFromFile(".\\assets\\images\\gameIcon.png");
    return icon;
}

sf::Window GameWindow() {
    sf::Window window;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::Image gameIcon = GameIcon();

    window.create(desktop, "John Pork: Fighter Hero");
    window.setIcon(gameIcon.getSize(), gameIcon.getPixelsPtr());
    return window;
}

int main() {
   
    sf::Window window = GameWindow();
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }

    return 0;
}