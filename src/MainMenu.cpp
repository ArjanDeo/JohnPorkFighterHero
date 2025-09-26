#include "Common.h"
#include "MainMenu.h"
#include <TGUI/TGUI.hpp>
#include "UI/Button.h"
MainMenu::MainMenu(sf::RenderWindow& window, AssetManager& assetManager, GameState& gameState)
    : window(window), assetManager(assetManager), font(assetManager.getFont("Raleway.ttf")), gameState(gameState) {}

void MainMenu::handleEvents(const std::optional<sf::Event>& event) {
    for (auto& btn : menuButtons) {
        btn.handleEvent(event);
    }
}
MainMenu::~MainMenu() {
	if (currentMusic.getStatus() == sf::Music::Status::Playing) {
		currentMusic.stop();
	}
	assetManager.DeleteSoundBuffer("main_menu_theme.ogg");
	assetManager.DeleteTexture("main_menu_background.jpg");
	menuButtons.clear();
}

void MainMenu::UpdateButtonLayout(sf::RectangleShape& overlay) {
    float startX = overlay.getPosition().x + ((overlay.getSize().x * (1 / 4096.f)));
    float startY = overlay.getPosition().y + 200;
    float spacing = 80;
    float buttonWidth = overlay.getSize().x;
    float buttonHeight = 60;

    for (size_t i = 0; i < menuButtons.size(); ++i) {
        menuButtons[i].setSize({ buttonWidth, buttonHeight });
        menuButtons[i].setPosition({ startX, startY + i * spacing });
    }
}

void MainMenu::InitializeMenuButtons(sf::RectangleShape& overlay) {
    Button missionButton(font, "Missions", 32);
    missionButton.setOnClick([this]() { gameState = GameState::MISSION_MENU; });

    Button settingsButton(font, "Settings", 32);
    settingsButton.setOnClick([this]() { gameState = GameState::SETTINGS_MENU; });

    Button exitButton(font, "Exit", 32);
    exitButton.setOnClick([this]() { window.close(); });

    menuButtons.push_back(missionButton);
    menuButtons.push_back(settingsButton);
    menuButtons.push_back(exitButton);
}

void MainMenu::DrawMainMenu() {
    sf::Vector2u winSize = window.getSize();
    sf::Texture& backgroundTexture = assetManager.getTexture("main_menu_background.jpg");
    sf::Sprite backgroundSprite(backgroundTexture);

    backgroundSprite.setScale(common.ScaleVector(backgroundTexture.getSize(), winSize));

    sf::RectangleShape overlay;

    sf::Color overlayColor(0, 0, 0, 200);
    overlay.setFillColor(overlayColor);

    overlay.setPosition(sf::Vector2f(winSize.x * 0.07f, 0.f));

    overlay.setSize({ winSize.x * 0.2f, static_cast<float>(winSize.y) });

    if (menuButtons.empty()) {
		InitializeMenuButtons(overlay);
    }
    

    UpdateButtonLayout(overlay);

    sf::Text title(font);

    title.setString("John Pork: Fighter Hero");
    title.setCharacterSize(static_cast<unsigned int>(overlay.getSize().x * 0.08));
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    title.setFillColor(sf::Color::White);
    title.setPosition({ overlay.getPosition().x + 35, overlay.getPosition().y + 20 });


    sf::SoundSource::Status musicStatus = currentMusic.getStatus();
    if (musicStatus != sf::SoundSource::Status::Playing) {
        common.PlayMusic(currentMusic, "assets/sound/main_menu_theme.ogg", true);
    }


    window.draw(backgroundSprite);
    window.draw(overlay);
    window.draw(title);
    for (auto& btn : menuButtons) {
        btn.draw(window);
	}
}