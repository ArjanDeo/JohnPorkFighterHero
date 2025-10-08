#pragma once
#include <SFML/Graphics.hpp>
#include "Window/BaseWindow.h"
#include "GameState.h"
#include "AssetManager.h"
#include "Ui/Button.h"

class SettingsMenu : public BaseWindow {
public:
	SettingsMenu(sf::RenderWindow& window, AssetManager& assetManager, GameState& gameState);
	void drawWindow() override;
	void handleEvent(const std::optional<sf::Event>& event) override;
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
	void updateElementLayout() override;
};