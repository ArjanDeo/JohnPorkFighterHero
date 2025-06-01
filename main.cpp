#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <windows.h>
#include <random>
#include <string>
#include <math.h>
#include "enemy.h"
#include "player.h"
#include "button.h"

void spawnEnemies(sf::Sprite(&enemies)[10], const sf::Sprite& playerSprite, const sf::Sprite& gun, const sf::RenderWindow& window)
{
    for (int i = 0; i < 10; ++i)
    {
        Enemy enemy("assets/tim_cheese.png", { 0.45f, 0.45f });
        enemies[i] = enemy.getSprite();
        sf::FloatRect playerBounds = playerSprite.getGlobalBounds();
        sf::FloatRect gunBounds = gun.getGlobalBounds();

        sf::Vector2u windowSize = window.getSize();
        float enemyWidth = enemies[i].getGlobalBounds().width;
        float enemyHeight = enemies[i].getGlobalBounds().height;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> distX(0.f, windowSize.x - enemyWidth);
        std::uniform_real_distribution<float> distY(0.f, windowSize.y - enemyHeight);

        sf::Vector2f spawnPos;

        do {
            spawnPos = { distX(gen), distY(gen) };
            enemies[i].setPosition(spawnPos);
        } while (
            enemies[i].getGlobalBounds().intersects(playerBounds) ||
            enemies[i].getGlobalBounds().intersects(gunBounds)
            );
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#pragma region Window Init
    sf::SoundBuffer rifleBuffer;
    sf::Sound rifleSound;

    sf::SoundBuffer gunshotBuffer;
    sf::Sound gunshotSound;

    if (!gunshotBuffer.loadFromFile("assets/gunshot.ogg")) return -1;
    gunshotSound.setBuffer(gunshotBuffer);
	gunshotSound.setVolume(50);

    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(desktopMode.width - 20, desktopMode.height - 20), "John Pork: Fighter Hero");

    sf::Image icon;
    if (!icon.loadFromFile("assets/john_pork_is_calling.jpg")) return -1;
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
#pragma endregion

#pragma region Characters / Sprites
    sf::Texture playerDefaultTexture;
    playerDefaultTexture.loadFromFile("assets/john_pork.png");
    Player player("assets/john_pork.png", "John Pork", 100);
    sf::Sprite playerSprite = player.getSprite();

    sf::Texture gunTexture;
    gunTexture.loadFromFile("assets/ak-47.png");
    sf::Sprite gun;
    gun.setTexture(gunTexture);
    gun.setPosition(static_cast<float>(window.getSize().x * 0.2f), static_cast<float>(window.getSize().y * 0.2f));
    gun.setScale(0.35f, 0.35f);

    sf::Sprite enemies[10];
	spawnEnemies(enemies, playerSprite, gun, window);
#pragma endregion

#pragma region Music + Font 
    sf::Texture background;
    background.loadFromFile("assets/background.jpg");
    sf::Sprite backgroundSprite(background);
    backgroundSprite.setScale(
        static_cast<float>(window.getSize().x) / background.getSize().x,
        static_cast<float>(window.getSize().y) / background.getSize().y
    );



    bool hasGun = false;
    sf::Texture playerWithGunTexture;
    playerWithGunTexture.loadFromFile("assets/john_pork_with_gun.png");

    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("assets/bullet.png");
    std::vector<sf::Sprite> bullets;
    std::vector<float> angles;

    sf::Font font;
    if (!font.loadFromFile("assets/porky.ttf")) return -1;
    sf::Text text("Objective: Eradicate Tim Cheese (If you die, game over)", font, 50);
    text.setFillColor(sf::Color::Magenta);

    sf::Text healthText("Health: 100", font, 50);
    healthText.setFillColor(sf::Color::Green);

    sf::Music music;
    if (!music.openFromFile("assets/LegionMusic.ogg")) return -1;
    music.setLoop(true);
    music.setVolume(20);
    music.play();

    sf::Music ringtone;
    if (!ringtone.openFromFile("assets/john_pork_is_calling.ogg")) return -1;
    ringtone.setLoop(true);
    music.setVolume(35);
    ringtone.play();

#pragma endregion

#pragma region Menu
    sf::RectangleShape menuBackground;
	menuBackground.setSize(sf::Vector2f(window.getSize()));
	menuBackground.setFillColor(sf::Color(0, 0, 0, 100)); // Semi-transparent black

    std::vector<Button> menuButtons = {
        Button("Close Game", 25, 25, font),
        Button("Restart Game", 25, 95.5, font),
    };

    bool menuState = false;

#pragma endregion

#pragma region Clocks
    sf::Clock deltaClock; 
    sf::Clock bulletClock;
    sf::Clock damageCooldownClocks[10];
    sf::Time damageCooldown = sf::seconds(1.f);
    float fireDelay = 0.2f;
#pragma endregion

#pragma region Game Loop
    while (window.isOpen()) {
        float dt = deltaClock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (menuState) {
                        music.play();
						ringtone.play();
						gunshotSound.play();
                    }
                    menuState = !menuState;
                }
            }
        }

        if (menuState) {
            window.clear();

            window.draw(backgroundSprite);
            if (!hasGun) window.draw(gun);
            for (int i = 0; i < 10; ++i) window.draw(enemies[i]);
            window.draw(playerSprite);
            for (int i = 0; i < bullets.size(); ++i) window.draw(bullets[i]);
            window.draw(healthText);
            window.draw(text);
            window.draw(menuBackground);

            for (auto& btn : menuButtons) {
                btn.draw(window);
            }
            music.pause();
			ringtone.pause();
			gunshotSound.pause();
            window.display();

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
                menuButtons[0].getShape().getGlobalBounds().contains(
                    static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
                window.close();
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
                menuButtons[1].getShape().getGlobalBounds().contains(
                    static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
                window.clear();
                window.draw(backgroundSprite);
                hasGun = false;

                if (!hasGun) window.draw(gun);
                
                playerSprite.setTexture(playerDefaultTexture);

                spawnEnemies(enemies, playerSprite, gun, window);
                for (int i = 0; i < 10; ++i) window.draw(enemies[i]);
                window.draw(playerSprite);
				bullets.clear();

				angles.clear();
				player.setHealth(100);
                window.draw(healthText);
                window.draw(text);
				music.play();
				ringtone.play();
				window.display();
				menuState = false;
                continue;
            }
            continue;
        }

        #pragma region Controls
        float speed = 500.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) playerSprite.move(0.f, -speed * dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) playerSprite.move(-speed * dt, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) playerSprite.move(0.f, speed * dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) playerSprite.move(speed * dt, 0.f);        
        #pragma endregion

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
                    if (!player.isAlive()) window.close();                    
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

#pragma region Gun Logic
        if (hasGun && sf::Mouse::isButtonPressed(sf::Mouse::Left) && bulletClock.getElapsedTime().asSeconds() > fireDelay) {

            gunshotSound.play();
            sf::Sprite bullet;
            bullet.setTexture(bulletTexture);
            bullets.push_back(bullet);
            bullets.back().setScale(0.09f, 0.09f);
            bullets.back().setOrigin(5, 5);
            sf::Vector2f center = {
            playerSprite.getPosition().x + playerSprite.getGlobalBounds().width / 2,
            playerSprite.getPosition().y + playerSprite.getGlobalBounds().height / 2
            };
            bullets.back().setPosition(center);

            float mouseX = sf::Mouse::getPosition(window).x;
            float mouseY = sf::Mouse::getPosition(window).y;
            sf::Vector2f playerCenter = {
                playerSprite.getPosition().x + playerSprite.getGlobalBounds().width / 2.f,
                playerSprite.getPosition().y + playerSprite.getGlobalBounds().height / 2.f
            };
            float angle = std::atan2(mouseY - playerCenter.y, mouseX - playerCenter.x);
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

#pragma endregion

        window.draw(playerSprite);
        window.draw(healthText);
        window.draw(text);
        window.display();
    }
#pragma endregion

    return 0;
}
