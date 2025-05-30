#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <windows.h>
#include "player.h"
#include <random>
#include <string>
#include <math.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#pragma region Init
    sf::SoundBuffer rifleBuffer;
    sf::Sound rifleSound;

    sf::SoundBuffer gunshotBuffer;
    sf::Sound gunshotSound;

    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(desktopMode.width, desktopMode.height), "John Pork: Fighter Hero");

    sf::Image icon;
    if (!icon.loadFromFile("assets/john_pork_is_calling.jpg")) return -1;
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    Player player("assets/john_pork.png", "John Pork", 100);
    sf::Sprite playerSprite = player.getSprite();

#pragma region Sprites

    sf::Sprite enemies[10];
    sf::Texture enemyTexture;
    enemyTexture.loadFromFile("assets/tim_cheese.png");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> xrange(0, window.getSize().x);
    std::uniform_int_distribution<> yrange(0, window.getSize().y);

    for (int i = 0; i < 10; ++i) {
        float x = static_cast<float>(xrange(gen));
        float y = static_cast<float>(yrange(gen));
        sf::Sprite enemy;
        enemy.setTexture(enemyTexture);
        enemy.setPosition(x, y);
        enemy.setScale(0.45f, 0.45f);

        sf::Vector2u windowSize = window.getSize();

        sf::FloatRect enemyBounds = enemy.getGlobalBounds();
        sf::FloatRect playerBounds = playerSprite.getGlobalBounds();

        auto isEnemyOutsideWindow = [&](const sf::FloatRect& bounds) {
            return bounds.left < 0.f ||
                bounds.top < 0.f ||
                bounds.left + bounds.width > static_cast<float>(windowSize.x) ||
                bounds.top + bounds.height > static_cast<float>(windowSize.y);
            };

        while (enemyBounds.intersects(playerBounds) || isEnemyOutsideWindow(enemyBounds)) {
            float x = static_cast<float>(xrange(gen));
            float y = static_cast<float>(yrange(gen));
            enemy.setPosition(x, y);

            enemyBounds = enemy.getGlobalBounds();
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

    sf::Texture gunTexture;
    gunTexture.loadFromFile("assets/ak-47.png");
    sf::Sprite gun;
    gun.setTexture(gunTexture);
    gun.setPosition(static_cast<float>(window.getSize().x * 0.2f), static_cast<float>(window.getSize().y * 0.2f));
    gun.setScale(0.35f, 0.35f);

    bool hasGun = false;
    sf::Texture playerWithGunTexture;
    playerWithGunTexture.loadFromFile("assets/john_pork_with_gun.png");

    sf::Texture bulletTexture;
	bulletTexture.loadFromFile("assets/bullet.png");
    std::vector<sf::Sprite> bullets;
    std::vector<float> angles;
#pragma endregion

    sf::Font font;
    if (!font.loadFromFile("assets/porky.ttf")) return -1;
    sf::Text text("Objective: Eradicate Tim Cheese (If you die, game over)", font, 50);
    text.setFillColor(sf::Color::Magenta);

    sf::Text healthText("Health: 100", font, 50);
    healthText.setFillColor(sf::Color::Green);

    sf::Music music;
    if (!music.openFromFile("assets/LegionMusic.ogg")) return -1;
    music.setLoop(true);
    music.setVolume(35);
    music.play();

    sf::Music ringtone;
    if (!ringtone.openFromFile("assets/john_pork_is_calling.ogg")) return -1;
    ringtone.setLoop(true);
    ringtone.play();

#pragma endregion

    sf::Clock deltaClock; 
    sf::Clock bulletClock;
    sf::Clock damageCooldownClocks[10];
    sf::Time damageCooldown = sf::seconds(1.f);
    float fireDelay = 0.2f;
    while (window.isOpen()) {
        float dt = deltaClock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }

        float speed = 500.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) playerSprite.move(0.f, -speed * dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) playerSprite.move(-speed * dt, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) playerSprite.move(0.f, speed * dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) playerSprite.move(speed * dt, 0.f);

        healthText.setPosition(0.f, 60.f);
        healthText.setString("Health: " + std::to_string(player.getHealth()));
        healthText.setFillColor(player.getHealth() <= 50 ? sf::Color::Red : sf::Color::Green);

        if (!hasGun && playerSprite.getGlobalBounds().intersects(gun.getGlobalBounds())) {
            hasGun = true;
            playerSprite.setTexture(playerWithGunTexture);

            if (!rifleBuffer.loadFromFile("assets/rifle_load.ogg")) return -1;
            rifleSound.setBuffer(rifleBuffer);
            rifleSound.play();
        }

        for (int i = 0; i < 10; ++i) {
            if (playerSprite.getGlobalBounds().intersects(enemies[i].getGlobalBounds())) {
                if (damageCooldownClocks[i].getElapsedTime() >= damageCooldown) {
                    player.takeDamage(10);
                    damageCooldownClocks[i].restart();
                    if (!player.isAlive()) {
                        window.close();
                    }
                }
            }
        }

        auto& pos = playerSprite.getPosition();
        auto bounds = playerSprite.getGlobalBounds();

        if (pos.x < 0.f) playerSprite.setPosition(0.f, pos.y);
        else if (pos.x + bounds.width > window.getSize().x)
            playerSprite.setPosition(window.getSize().x - bounds.width, pos.y);

        if (pos.y < 0.f) playerSprite.setPosition(pos.x, 0.f);
        else if (pos.y + bounds.height > window.getSize().y)
            playerSprite.setPosition(pos.x, window.getSize().y - bounds.height);

        window.clear();
        window.draw(backgroundSprite);
        if (!hasGun) window.draw(gun);
        bool allEnemiesRemoved = true;
        sf::Vector2f offScreenPos(window.getSize().x + 100.f, window.getSize().y + 100.f);

        for (int i = 0; i < 10; ++i) {
            if (enemies[i].getPosition() != offScreenPos) {
                allEnemiesRemoved = false;
                break;
            }
        }

        if (allEnemiesRemoved) {
            text.setString("You Win!");
            text.setFillColor(sf::Color::Green);
        }
        for (int i = 0; i < 10; ++i) window.draw(enemies[i]);
        window.draw(playerSprite);
        window.draw(healthText);
        window.draw(text);

		if (hasGun && sf::Mouse::isButtonPressed(sf::Mouse::Left) && bulletClock.getElapsedTime().asSeconds() > fireDelay) {
            if (!gunshotBuffer.loadFromFile("assets/gunshot.ogg")) return -1;
            gunshotSound.setBuffer(gunshotBuffer);
            gunshotSound.play();
			sf::Sprite bullet;
            bullet.setTexture(bulletTexture);
            bullets.push_back(bullet);
            bullets.back().setScale(0.09f, 0.09f);
            bullets.back().setOrigin(5, 5);
            bullets.back().setPosition(playerSprite.getPosition());

			float mouseX = sf::Mouse::getPosition(window).x;
			float mouseY = sf::Mouse::getPosition(window).y;
			float angle = std::atan2(mouseY - playerSprite.getPosition().y, mouseX - playerSprite.getPosition().x);
            bullets.back().setRotation(angle * 180 / 3.14159f);
			angles.push_back(angle);
			bulletClock.restart();
		}

        float bulletSpeed = 1000.f;

        for (int i = (int)bullets.size() - 1; i >= 0; --i) {           
            bullets[i].move(bulletSpeed * dt * cos(angles[i]), bulletSpeed * dt * sin(angles[i]));

            sf::Vector2f pos = bullets[i].getPosition();
            if (pos.x < 0 || pos.x > window.getSize().x || pos.y < 0 || pos.y > window.getSize().y) {
                bullets.erase(bullets.begin() + i);
                angles.erase(angles.begin() + i);
                continue;
            }

            bool bulletRemoved = false;
            for (int j = 0; j < 10; ++j) {
                if (bullets[i].getGlobalBounds().intersects(enemies[j].getGlobalBounds())) {
                    // Respawn enemy avoiding player:
                    sf::Vector2f newPos;
                    do {
                        newPos = { static_cast<float>(window.getSize().x + 100), static_cast<float>(window.getSize().y + 100) };
                        enemies[j].setPosition(newPos);
                    } while (enemies[j].getGlobalBounds().intersects(playerSprite.getGlobalBounds()));

                    bullets.erase(bullets.begin() + i);
                    angles.erase(angles.begin() + i);
                    bulletRemoved = true;
                    break;
                }
            }
            if (!bulletRemoved) {
                window.draw(bullets[i]);
            }
        }


        
        window.display();
    }

    return 0;
}
