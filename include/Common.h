#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Common {
public:
    static sf::Vector2f ScaleVector(sf::Vector2u child, sf::Vector2u parent);
    void PlayMusic(sf::Music& currentMusic, std::string sourceFile, bool looping);
};