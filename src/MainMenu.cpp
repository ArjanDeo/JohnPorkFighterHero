#include "Common.h"
#include "MainMenu.h"
MainMenu::MainMenu(sf::RenderWindow& window, AssetManager& assetManager, GameState& gameState)
    : window(window), assetManager(assetManager), font(assetManager.getFont("Raleway.ttf")), gameState(gameState) {}

void MainMenu::handleEvents(const std::optional<sf::Event>& event) {
    for (auto& btn : menuButtons) {
        if (btn.isClicked(window, event)) {
            if (btn.getText() == "Exit")
                window.close();
            else if (btn.getText() == "Settings") {
                gameState = GameState::SETTINGS_MENU;
            }
            else if (btn.getText() == "Missions") {
                // do nothing as of yet
            }
        }
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
    std::vector<std::string> labels = { "Missions", "Settings", "Exit" };

    float startX = overlay.getGlobalBounds().position.x + ((overlay.getGlobalBounds().size.x * (static_cast<float>(1) / 4096)));
    float startY = overlay.getGlobalBounds().position.y + 200;
    float spacing = 80;          // space between buttons
    float buttonWidth = overlay.getGlobalBounds().size.x;
    float buttonHeight = 60;

    for (size_t i = 0; i < labels.size(); ++i) {
        Button btn(font, labels[i]);
        btn.setSize({ buttonWidth, buttonHeight });
        btn.setFillColor(sf::Color(50, 50, 50, 200)); // semi-transparent
        btn.setPosition({ startX, startY + i * spacing });

        menuButtons.push_back(btn);
    }
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

    // Initialize buttons once
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

    for (auto& btn : menuButtons) {
		
    }

    window.draw(backgroundSprite);
    window.draw(overlay);
    window.draw(title);
    for (auto& btn : menuButtons) btn.draw(window);
}