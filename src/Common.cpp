#include "Common.h"
#include <iostream>
#include <SFML/Audio.hpp>
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
    currentMusic.play();
}