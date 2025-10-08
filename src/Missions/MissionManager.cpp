#include "Missions/MissionManager.h"
#include "AssetManager.h"
MissionManager::MissionManager(AssetManager &assetManager) : assetManager(assetManager) {
	sf::Texture prologueBackground = assetManager.getTexture("prologue_background.png");
	sf::Texture prologueThumbnail = assetManager.getTexture("prologue_thumbnail.png");
	textures.push_back(prologueBackground);
	textures.push_back(prologueThumbnail);
	MissionSelection prologue = { 1, textures[0], textures[1], "Prologue", "Pursue Tim Cheese", MissionDifficulty::EASY};

	missions.push_back(prologue);
}

const std::vector<MissionSelection>& MissionManager::getMissions() const {
    return missions;
}
MissionManager::~MissionManager() {
	for (const auto& mission : missions) {
		assetManager.DeleteTexture(mission.missionName + "_background.png");
		assetManager.DeleteTexture(mission.missionName + "_thumbnail.png");
	}
}