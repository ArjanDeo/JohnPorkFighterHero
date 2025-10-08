#pragma once
#include "MissionSelection.h"
#include "AssetManager.h"
class MissionManager {
public:
	MissionManager(AssetManager &assetManager);
	~MissionManager();
	const std::vector<MissionSelection>& getMissions() const;
private:
	std::vector<MissionSelection> missions;
	std::vector<sf::Texture> textures;
	AssetManager& assetManager;
};
