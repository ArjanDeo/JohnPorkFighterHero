#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "Button.h"
#include "GameState.h"
AssetManager assetManager("assets");
sf::Music currentMusic;
sf::Font &font = assetManager.getFont("Raleway.ttf");
std::vector<Button> menuButtons;


sf::Image GameIcon() {
    sf::Texture &iconTexture = assetManager.getTexture("gameIcon.png");
    
    sf::Image iconImage = iconTexture.copyToImage();
    
    return iconImage;
}

sf::RenderWindow GameWindow() {
    sf::RenderWindow window;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::Image gameIcon = GameIcon();

    window.create(desktop, "John Pork: Fighter Hero");
    window.setIcon(gameIcon.getSize(), gameIcon.getPixelsPtr());
    return window;
}

sf::Vector2f ScaleVector(sf::Vector2u child, sf::Vector2u parent) {
    float scaleX = static_cast<float>(parent.x) / child.x;
    float scaleY = static_cast<float>(parent.y) / child.y;
    sf::Vector2f resultVector(scaleX, scaleY);
    return resultVector;
}

void PlayMusic(std::string sourceFile, bool looping) {
    if (!currentMusic.openFromFile(sourceFile)) {
        std::cerr << "Failed to load music: " << sourceFile << std::endl;
        return;
    }
    
    currentMusic.setLooping(looping);
    currentMusic.play();
}

void UpdateButtonLayout(sf::RectangleShape &overlay) {
    float startX = overlay.getPosition().x + ((overlay.getSize().x * (1 / 4096.f)));
    float startY = overlay.getPosition().y + 200;
    float spacing = 80;
    float buttonWidth = overlay.getSize().x;
    float buttonHeight = 60;

    for (size_t i = 0; i < menuButtons.size(); ++i) {
        menuButtons[i].shape.setSize({buttonWidth, buttonHeight});
        menuButtons[i].shape.setPosition({startX, startY + i * spacing});

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


void InitializeMenuButtons(sf::RectangleShape &overlay) {
     std::vector<std::string> labels = { "New Game", "Load Game", "Settings", "Exit" };

    float startX = overlay.getGlobalBounds().position.x + ((overlay.getGlobalBounds().size.x * (1 / 4096)));
    float startY = overlay.getGlobalBounds().position.y + 200;
    float spacing = 80;          // space between buttons
    float buttonWidth = overlay.getGlobalBounds().size.x;
    float buttonHeight = 60;

    for (size_t i = 0; i < labels.size(); ++i) {
        Button btn(font, labels[i]);
        btn.shape.setSize({buttonWidth, buttonHeight});
        btn.shape.setFillColor(sf::Color(50, 50, 50, 200)); // semi-transparent
        btn.shape.setPosition({startX, startY + i * spacing});

        btn.text.setFont(font);
        btn.text.setString(labels[i]);
        btn.text.setCharacterSize(36);
        btn.text.setFillColor(sf::Color::White);

        // center text inside button
        sf::FloatRect textBounds = btn.text.getLocalBounds();
        btn.text.setOrigin({textBounds.position.x + textBounds.size.x / 2.f,
                        textBounds.position.y + textBounds.size.y / 2.f});
        btn.text.setPosition({btn.shape.getPosition().x + buttonWidth / 2.0f,
                            btn.shape.getPosition().y + buttonHeight / 2.0f});
        menuButtons.push_back(btn);
    }
}

void DrawMainMenu(sf::RenderWindow &window) {
    sf::Vector2u winSize = window.getSize();
    sf::Texture &backgroundTexture = assetManager.getTexture("main_menu_background.jpg");
    sf::Sprite backgroundSprite(backgroundTexture);

    backgroundSprite.setScale(ScaleVector(backgroundTexture.getSize(), winSize));

    sf::RectangleShape overlay;

    sf::Color overlayColor(0, 0, 0, 200);
    overlay.setFillColor(overlayColor);

    overlay.setPosition(sf::Vector2f(winSize.x * 0.07f, 0.f));

    overlay.setSize({winSize.x * 0.2f, static_cast<float>(winSize.y)});
    
    // Initialize buttons once
if (menuButtons.empty()) {
    std::vector<std::string> labels = { "New Game", "Load Game", "Settings", "Exit" };
    for (auto &label : labels) {
        menuButtons.push_back(Button(font, label));
    }
}

// Update layout every frame (or on resize)
UpdateButtonLayout(overlay);

    sf::Text title(font);

    title.setString("John Pork: Fighter Hero");
    title.setCharacterSize(40);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    title.setFillColor(sf::Color::White);
    title.setPosition({overlay.getPosition().x + 20, overlay.getPosition().y + 20});

    
    sf::SoundSource::Status musicStatus = currentMusic.getStatus();
    if (musicStatus != sf::SoundSource::Status::Playing) {
        PlayMusic("assets/sound/main_menu_theme.wav", true);
    }

    for (auto &btn : menuButtons) {
        if (btn.clicked)
            btn.shape.setFillColor({51, 158, 158, 200});
        else if (btn.isMouseOver(window))
            btn.shape.setFillColor({100, 100, 100, 200});
        else
            btn.shape.setFillColor({50, 50, 50, 200});
    }
   
    window.draw(backgroundSprite);
    window.draw(overlay);
    window.draw(title);
    for (auto &btn : menuButtons) btn.draw(window);
    
}

int main() {
    GameState gameState = GameState::MAIN_MENU;

    sf::RenderWindow window = GameWindow();
    while (window.isOpen())
    {
        window.clear();
        switch (gameState)
        {
        case GameState::MAIN_MENU:
            DrawMainMenu(window);
            break;
        
        default:
            break;
        }
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
            if (const auto *windowResized = event->getIf<sf::Event::Resized>()) {
                sf::FloatRect visibleArea(
                    sf::Vector2f(0.f, 0.f),
                    sf::Vector2f(windowResized->size.x, windowResized->size.y)
                );

                window.setView(sf::View(visibleArea));
            } 
           for (auto &btn : menuButtons) {
            if (btn.isClicked(window, event)) {
                if (btn.text.getString() == "Exit")
                    window.close();
                else
                    btn.shape.setFillColor(sf::Color::Magenta);
            }
           } 
        }
        window.display();
    }

    return 0;
}