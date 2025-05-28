#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <windows.h>
#include "player.h"
#include <random>
#include <string>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {
#pragma region Init

    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(desktopMode.width, desktopMode.height), "John Pork: Fighter Hero");
    sf::Image icon;
    if (!icon.loadFromFile("assets/john_pork_is_calling.jpg")) {
		return -1;
    }
    else {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }
    Player player("assets/john_pork.png", "John Pork", 100);
    sf::Sprite& playerSprite = player.getSprite();

    sf::Sprite enemies[10];
    sf::Texture enemyTexture;
    enemyTexture.loadFromFile("assets/tim_cheese.png");

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> xrange(0, window.getSize().x);
    std::uniform_int_distribution<> yrange(0, window.getSize().y);

    for (int i = 0; i < 10; ++i)
    {
        float x = static_cast<float>(xrange(gen));
        float y = static_cast<float>(yrange(gen));

        sf::Sprite enemy;
        
		enemy.setTexture(enemyTexture);
        enemy.setPosition(x, y);
		enemy.setScale(0.45f, 0.45f);

		if (enemy.getGlobalBounds().intersects(playerSprite.getGlobalBounds())) {
			while (enemy.getGlobalBounds().intersects(playerSprite.getGlobalBounds())) {
				x = static_cast<float>(xrange(gen));
				y = static_cast<float>(yrange(gen));
				enemy.setPosition(x, y);
			}
		}
        enemies[i] = enemy;
    }
    sf::Texture background;
	background.loadFromFile("assets/background.jpg");
	sf::Sprite backgroundSprite(background);
	backgroundSprite.setScale(
		static_cast<float>(window.getSize().x) / background.getSize().x,
		static_cast<float>(window.getSize().y) / background.getSize().y
	);
	backgroundSprite.setPosition(0.f, 0.f);
    // Text + Font

    sf::Font font;
    if (!font.loadFromFile("assets/porky.ttf")) {
        return -1;
    }
    const char textContent[57] = "Objective: Eradicate Tim Cheese (If you die, game over)";
    sf::Text text(textContent, font, 50);
	text.setFillColor(sf::Color::Magenta);

	std::string healthTextString = "Health: " + std::to_string(player.getHealth());
    sf::Text healthText(healthTextString.c_str(), font, 50);
	
    // Music
    sf::Music music;
    if (!music.openFromFile("assets/LegionMusic.ogg")) {
        return -1;
    }
	music.setLoop(true);
	music.setVolume(35);
    music.play();
    sf::Music ringtone;
    if (!ringtone.openFromFile("assets/john_pork_is_calling.ogg")) {
        return -1;
    }
    ringtone.setLoop(true);
    ringtone.play();
#pragma endregion
    sf::Clock damageCooldownClocks[10];
    sf::Time damageCooldown = sf::seconds(1.f); // 1 second cooldown per enemy

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) playerSprite.move(0.f, -0.1f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) playerSprite.move(-0.1f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) playerSprite.move(0.f, 0.1f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) playerSprite.move(0.1f, 0.f);        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();

        healthText.setPosition(0.f, 60.f);
        healthText.setFillColor(sf::Color::Green);
		healthText.setString("Health: " + std::to_string(player.getHealth()));
		if (player.getHealth() <= 50) {
			healthText.setFillColor(sf::Color::Red);
		}
        window.clear();
        window.draw(backgroundSprite);

        for (int i = 0; i < 10; ++i) {
            window.draw(enemies[i]);
        }
        window.draw(playerSprite);
		window.draw(healthText);
		window.draw(text);
        window.display();

		// Collision detection
        for (int i = 0; i < 10; ++i) {
            if (playerSprite.getGlobalBounds().intersects(enemies[i].getGlobalBounds())) {
                if (damageCooldownClocks[i].getElapsedTime() >= damageCooldown) {
                    player.takeDamage(10);
                    std::cout << "Player took damage from enemy " << i << "! Current health: " << player.getHealth() << std::endl;
                    damageCooldownClocks[i].restart();

                    if (!player.isAlive()) {
                        std::cout << "Player is dead!" << std::endl;
                        window.close();
                    }
                }
            }
        }
        // Boundaries
        auto& pos = playerSprite.getPosition();
        auto bounds = playerSprite.getGlobalBounds();

        if (pos.x < 0.f)
            playerSprite.setPosition(0.f, pos.y);
        else if (pos.x + bounds.width > window.getSize().x)
            playerSprite.setPosition(window.getSize().x - bounds.width, pos.y);

        if (pos.y < 0.f)
            playerSprite.setPosition(playerSprite.getPosition().x, 0.f);
        else if (pos.y + bounds.height > window.getSize().y)
            playerSprite.setPosition(playerSprite.getPosition().x, window.getSize().y - bounds.height);

    }

    return 0;
}
