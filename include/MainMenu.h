#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <SFML/Audio.hpp>
#include <optional>
#include <string>
#include <map>
#include <iostream>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "Window/BaseWindow.h"
#include "Common.h"
#include "UI/Button.h"
#include "GameState.h"
#include "AssetManager.h"

class MainMenu : public BaseWindow {
public:
	MainMenu(sf::RenderWindow& window, AssetManager& assetManager, GameState& gameState);
	~MainMenu();
	void drawWindow() override;
	void handleEvent(const std::optional<sf::Event>& event) override;

private:
	Common common;
	GameState& gameState;
	sf::RenderWindow& window;
	AssetManager& assetManager;
	sf::Font& font;
	sf::RectangleShape overlay;
	std::vector<Button> menuButtons;
	sf::Music currentMusic;
	void updateElementLayout() override;
	void InitializeMenuButtons(sf::RectangleShape& overlay);
};