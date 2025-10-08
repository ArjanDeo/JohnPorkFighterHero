#include "Common.h"
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
sf::Vector2f Common::ScaleVector(sf::Vector2u child, sf::Vector2u parent) {
    float scaleX = static_cast<float>(parent.x) / child.x;
    float scaleY = static_cast<float>(parent.y) / child.y;
    sf::Vector2f resultVector(scaleX, scaleY);
    return resultVector;
}

void Common::PlayMusic(sf::Music &currentMusic,std::string sourceFile, bool looping) {
    if (!currentMusic.openFromFile(sourceFile)) {
        std::cerr << "Failed to load music: " << sourceFile << std::endl;
        return;
    }

    currentMusic.setLooping(looping);
	currentMusic.setVolume(50); // Set volume to 50%
    currentMusic.play();
}
void Common::ToggleFullscreen(sf::RenderWindow& window) {
    // Keep current settings
    sf::ContextSettings settings = window.getSettings();

    // Close the current window
    window.close();

    if (!isFullscreen) {
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        window.create(desktop, "John Pork: Fighter Hero", sf::Style::None, sf::State::Fullscreen, settings);
        isFullscreen = true;
    }
    else {
        window.create(sf::VideoMode({ 1280, 720 }), "John Pork: Fighter Hero", sf::Style::Default, sf::State::Windowed, settings);
        isFullscreen = false;
    }

}
