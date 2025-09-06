#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <SFML/Audio.hpp>
#include <optional>
#include <string>
#include <map>
#include <iostream>
#include "Common.h"
#include "Button.h"
#include "GameState.h"
#include "AssetManager.h"


class MainMenu {
public:
	MainMenu(sf::RenderWindow& window, AssetManager& assetManager, GameState& gameState);
	~MainMenu();
	void DrawMainMenu();
	void handleEvents(const std::optional<sf::Event>& event);

private:
	Common common;
	GameState& gameState;
	sf::RenderWindow& window;
	AssetManager& assetManager;
	sf::Font& font;
	sf::RectangleShape overlay;
	std::vector<Button> menuButtons;
	sf::Music currentMusic;
	void UpdateButtonLayout(sf::RectangleShape& overlay);
	void InitializeMenuButtons(sf::RectangleShape& overlay);
};