#pragma once
#include <SFML/Graphics.hpp>
enum MissionDifficulty {
	EASY,
	MEDIUM,
	HARD
};
struct MissionSelection {
    int missionId;
    sf::Texture& backgroundTexture;
    sf::Texture& thumbnailTexture;
    std::string missionName;
    std::string missionDescription;
    MissionDifficulty difficulty;
};
