#include "Window/BaseWindow.h"
#include "Missions/MissionManager.h"
#include "UI/MissionCard.h"
class MissionMenu : public BaseWindow {
	public:
		MissionMenu(sf::RenderWindow& window, AssetManager& assetManager, GameState& gameState);
		~MissionMenu();
		void drawWindow() override;
		void handleEvent(const std::optional<sf::Event>& event) override;

	private:
		GameState& gameState;
		sf::RenderWindow& window;
		AssetManager& assetManager;
		MissionManager missionManager;
		sf::Font& font;

		std::vector<MissionCard> cards;
		int selectedMissionIndex = -1;

		void initializeCards();
		void updateElementLayout() override;
		void selectMission(int index);
};