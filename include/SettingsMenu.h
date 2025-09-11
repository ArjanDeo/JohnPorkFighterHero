#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "AssetManager.h"
#include "Button.h"

class SettingsMenu {
public:
	SettingsMenu(sf::RenderWindow& window, AssetManager& assetManager, GameState& gameState);
	void DrawSettingsMenu();
	void handleEvents(const std::optional<sf::Event>& event);
private:
	GameState& gameState;
	sf::RenderWindow& window;
	AssetManager& assetManager;
	sf::Font& font;
	sf::RectangleShape overlay;
	std::vector<Button> settingsButtons;
	std::vector<std::string> labels = { "Vsync", "Fullscreen", "Close" };
	sf::Vector2u windowSize;

	void InitializeButtons();
};