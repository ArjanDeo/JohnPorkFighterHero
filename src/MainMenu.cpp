#include "Common.h"
#include "MainMenu.h"
MainMenu::MainMenu(sf::RenderWindow& window, AssetManager& assetManager) 
	: window(window), assetManager(assetManager), font(assetManager.getFont("Raleway.ttf")) 
{


}
void MainMenu::handleEvents(const std::optional<sf::Event>& event) {
    for (auto& btn : menuButtons) {
        if (btn.isClicked(window, event)) {
            if (btn.text.getString() == "Exit")
                window.close();
            else
                btn.shape.setFillColor(sf::Color::Magenta);
        }
    }
}
MainMenu::~MainMenu() {
	if (currentMusic.getStatus() == sf::Music::Status::Playing) {
		currentMusic.stop();
	}
}

void MainMenu::UpdateButtonLayout(sf::RectangleShape& overlay) {
    float startX = overlay.getPosition().x + ((overlay.getSize().x * (1 / 4096.f)));
    float startY = overlay.getPosition().y + 200;
    float spacing = 80;
    float buttonWidth = overlay.getSize().x;
    float buttonHeight = 60;

    for (size_t i = 0; i < menuButtons.size(); ++i) {
        menuButtons[i].shape.setSize({ buttonWidth, buttonHeight });
        menuButtons[i].shape.setPosition({ startX, startY + i * spacing });

        // center text
        sf::FloatRect textBounds = menuButtons[i].text.getLocalBounds();
        menuButtons[i].text.setOrigin({
            textBounds.position.x + textBounds.size.x / 2.f,
            textBounds.position.y + textBounds.size.y / 2.f
            });
        menuButtons[i].text.setPosition({
            menuButtons[i].shape.getPosition().x + buttonWidth / 2.f,
            menuButtons[i].shape.getPosition().y + buttonHeight / 2.f
            });
    }
}

void MainMenu::InitializeMenuButtons(sf::RectangleShape& overlay) {
    std::vector<std::string> labels = { "New Game", "Load Game", "Settings", "Exit" };

    float startX = overlay.getGlobalBounds().position.x + ((overlay.getGlobalBounds().size.x * (1 / 4096)));
    float startY = overlay.getGlobalBounds().position.y + 200;
    float spacing = 80;          // space between buttons
    float buttonWidth = overlay.getGlobalBounds().size.x;
    float buttonHeight = 60;

    for (size_t i = 0; i < labels.size(); ++i) {
        Button btn(font, labels[i]);
        btn.shape.setSize({ buttonWidth, buttonHeight });
        btn.shape.setFillColor(sf::Color(50, 50, 50, 200)); // semi-transparent
        btn.shape.setPosition({ startX, startY + i * spacing });

        btn.text.setFont(font);
        btn.text.setString(labels[i]);
        btn.text.setCharacterSize(36);
        btn.text.setFillColor(sf::Color::White);

        // center text inside button
        sf::FloatRect textBounds = btn.text.getLocalBounds();
        btn.text.setOrigin({ textBounds.position.x + textBounds.size.x / 2.f,
                        textBounds.position.y + textBounds.size.y / 2.f });
        btn.text.setPosition({ btn.shape.getPosition().x + buttonWidth / 2.0f,
                            btn.shape.getPosition().y + buttonHeight / 2.0f });
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
        std::vector<std::string> labels = { "Missions", "Settings", "Exit" };
        for (auto& label : labels) {
            menuButtons.push_back(Button(font, label));
        }
    }

    // Update layout every frame (or on resize)
    UpdateButtonLayout(overlay);

    sf::Text title(font);

    title.setString("John Pork: Fighter Hero");
    title.setCharacterSize(static_cast<unsigned int>(overlay.getSize().x * 0.08));
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    title.setFillColor(sf::Color::White);
    title.setPosition({ overlay.getPosition().x + 35, overlay.getPosition().y + 20 });


    sf::SoundSource::Status musicStatus = currentMusic.getStatus();
    if (musicStatus != sf::SoundSource::Status::Playing) {
        common.PlayMusic(currentMusic, "assets/sound/main_menu_theme.wav", true);
    }

    for (auto& btn : menuButtons) {
        if (btn.clicked)
            btn.shape.setFillColor({ 51, 158, 158, 200 });
        else if (btn.isMouseOver(window))
            btn.shape.setFillColor({ 100, 100, 100, 200 });
        else
            btn.shape.setFillColor({ 50, 50, 50, 200 });
    }

    window.draw(backgroundSprite);
    window.draw(overlay);
    window.draw(title);
    for (auto& btn : menuButtons) btn.draw(window);
}