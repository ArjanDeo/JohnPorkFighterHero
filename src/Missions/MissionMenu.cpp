#include "MissionMenu.h"
#include "Missions/MissionManager.h"
#include "UI/MissionCard.h"

MissionMenu::MissionMenu(sf::RenderWindow& window, AssetManager& assetManager, GameState& gameState) :
    gameState(gameState),
    window(window),
    assetManager(assetManager),
    font(assetManager.getFont("raleway.ttf")),
	missionManager(assetManager), BaseWindow(window, assetManager, gameState)
{
    updateElementLayout();
    initializeCards();
}

void MissionMenu::initializeCards() {
    cards.clear();
    int cardWidth = 700;
    int cardHeight = 300;
    int spacing = 15;
    int startY = (window.getSize().y - ((static_cast<unsigned long long>(cardHeight) + spacing) * missionManager.getMissions().size() - spacing)) / 2;

    for (size_t i = 0; i < missionManager.getMissions().size(); ++i) {
        const MissionSelection& mission = missionManager.getMissions()[i];

        // Create mission card
        MissionCard card(font, mission);
        card.setSize({ static_cast<float>(cardWidth), static_cast<float>(cardHeight) });
        card.setPosition({
            (window.getSize().x - cardWidth) / 2.0f,
            static_cast<float>(startY + i * (cardHeight + spacing))
            });

        // Set click callback to select this mission
        card.setOnClick([this, i]() {
            selectMission(i);
            });

        cards.push_back(card);
    }
}

void MissionMenu::updateElementLayout() {
    int cardWidth = 700;   // Bigger width
    int cardHeight = 280;  // Bigger height
    int spacing = 20;      // Horizontal spacing between cards

    // Calculate total width needed for all cards
    int totalWidth = (cardWidth * cards.size()) + (spacing * (static_cast<int>(cards.size()) - 1));
    int startX = (window.getSize().x - totalWidth) / 2;  // Center horizontally
    int centerY = (window.getSize().y - cardHeight) / 2; // Center vertically

    for (size_t i = 0; i < cards.size(); ++i) {
        cards[i].setSize({ static_cast<float>(cardWidth), static_cast<float>(cardHeight) });
        cards[i].setPosition({
            static_cast<float>(startX + i * (cardWidth + spacing)),  // Horizontal positioning
            static_cast<float>(centerY)  // Vertical centering
            });
    }
}

void MissionMenu::selectMission(int index) {
    // Deselect all cards first
    for (auto& card : cards) {
        card.setSelected(false);
    }

    // Select the clicked card
    if (index >= 0 && index < cards.size()) {
        cards[index].setSelected(true);
        selectedMissionIndex = index;

        // You can add logic here to start the mission or show mission details
        // For example:
        // gameState.setCurrentMission(missionManager.getMissions()[index]);
        // gameState.setState(GameState::State::PLAYING);
    }
}

void MissionMenu::drawWindow() {
    for (auto& card : cards) {
        card.draw(window);
    }
}

void MissionMenu::handleEvent(const std::optional<sf::Event>& event) {
    for (auto& card : cards) {
        card.handleEvent(event);
    }
}

MissionMenu::~MissionMenu() {
    cards.clear();
}