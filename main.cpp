#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <windows.h>
#include <random>
#include <string>
#include <math.h>
#include <zstd.h>
#include "enemy.h"
#include "player.h"
#include "button.h"
#include "assetManager.h"
// READ: REDO THE ASSET MANAGER TO ONLY TAKE CARE OF LOW LEVEL SHIT LIKE TEXTURES ETC, SPRITES AND TEXT ETC SHOULD BE MADE HERE!!
AssetManager assetManager("assets");
static sf::Vector2f generateRandomVector2f(sf::RenderWindow& window) {
    float marginX = window.getSize().x * 0.2;
    float marginY = window.getSize().y * 0.2;

    std::uniform_real_distribution<float> distX(0.0f, window.getSize().x - marginX);
    std::uniform_real_distribution<float> distY(0.0f, window.getSize().y - marginY);
    std::random_device rd;
    std::mt19937 gen(rd());

    sf::Vector2f spawnPos;
    return { distX(gen), distY(gen) };
}

static void spawnEnemies(sf::Sprite(&enemies)[10], const sf::Sprite& playerSprite, const sf::Sprite& gun, sf::RenderWindow& window)
{
    for (int i = 0; i < 10; ++i)
    {
        Enemy enemy("tim_cheese.png", assetManager, { 0.45f, 0.45f });
        enemies[i] = enemy.getSprite();
        sf::FloatRect playerBounds = playerSprite.getGlobalBounds();
        sf::FloatRect gunBounds = gun.getGlobalBounds();
        sf::Vector2u windowSize = window.getSize();
        float enemyWidth = enemies[i].getGlobalBounds().width;
        float enemyHeight = enemies[i].getGlobalBounds().height;

        do {
            enemies[i].setPosition(generateRandomVector2f(window));
        } while (
            enemies[i].getGlobalBounds().intersects(playerBounds) ||
            enemies[i].getGlobalBounds().intersects(gunBounds)
            );

    }
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#pragma region Window Init

    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(desktopMode.width - 20, desktopMode.height - 20), "John Pork: Fighter Hero");

	sf::SoundBuffer& rifleBuffer = assetManager.getSoundBuffer("rifle_load.ogg");
    sf::Sound rifleSound(rifleBuffer);

	sf::SoundBuffer& gunshotBuffer = assetManager.getSoundBuffer("gunshot.ogg");
	sf::Sound gunshotSound(gunshotBuffer);
	gunshotSound.setVolume(50);   

    sf::Image icon;
    if (!icon.loadFromFile("assets/meta/john_pork_is_calling.jpg")) return -1;
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
#pragma endregion

#pragma region Characters / Sprites
	sf::Texture& playerDefaultTexture = assetManager.getTexture("john_pork.png");

    Player player(playerDefaultTexture, "John Pork", 100, assetManager);
    sf::Sprite playerSprite = player.getSprite();

    sf::Texture& gunTexture = assetManager.getTexture("ak-47.png");
	sf::Sprite gun(gunTexture); 
    gun.setPosition(generateRandomVector2f(window));
    gun.setScale(0.4f, 0.4f);

    sf::Sprite enemies[10];
	spawnEnemies(enemies, playerSprite, gun, window);
#pragma endregion

#pragma region Music + Font 

    sf::Texture& backgroundTextureHQ = assetManager.getTexture("background-4k.jpg");
    sf::Texture& backgroundTexture = assetManager.getTexture("background.jpg");
	sf::Sprite backgroundSprite(backgroundTexture);
    
    backgroundSprite.setScale(
        static_cast<float>(window.getSize().x) / backgroundSprite.getTexture()->getSize().x,
        static_cast<float>(window.getSize().y) / backgroundSprite.getTexture()->getSize().y
    );
	sf::Font& porkyFont = assetManager.getFont("porky.ttf");

	sf::Text text("Objective: Eradicate Tim Cheese (If you die, game over)", porkyFont);
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::Magenta);

    sf::Text healthText("Health: 100", porkyFont);
    healthText.setCharacterSize(50);
    healthText.setFillColor(sf::Color::Green);

    sf::Music music;
	music.openFromFile("assets/sound/LegionMusic.ogg");
    music.setLoop(true);
    music.setVolume(20);
    music.play();

    sf::Music ringtone;
    ringtone.openFromFile("assets/sound/john_pork_is_calling.ogg");
    ringtone.setLoop(true);
    ringtone.setVolume(35);
    ringtone.play();
#pragma endregion

#pragma region Menu
    bool useHighQualityTextures = false;
    sf::RectangleShape menuBackground;
	menuBackground.setSize(sf::Vector2f(window.getSize()));
	menuBackground.setFillColor(sf::Color(0, 0, 0, 100)); // Semi-transparent black

    std::vector<Button> menuButtons = {
        Button("Close Game", 25, 25, porkyFont),
        Button("Restart Game", 25, 95.5, porkyFont),
        Button("HD Textures: " + (useHighQualityTextures ? std::string("On") : "Off"), 25, 166, porkyFont),

    };

    bool menuState = false;

#pragma endregion

#pragma region Clocks & Variables
    std::vector<sf::Sprite> bullets;
    std::vector<float> angles;
    sf::Clock deltaClock; 
    sf::Clock bulletClock;
    sf::Clock damageCooldownClocks[10];
    sf::Time damageCooldown = sf::seconds(1.f);
    float fireDelay = 0.2f;
    bool hasGun = false;
    bool movingUp = false;
    bool movingDown = false;
    bool movingLeft = false;
    bool movingRight = false;
#pragma endregion

#pragma region Game Loop
    while (window.isOpen()) {
        float dt = deltaClock.restart().asSeconds();       
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Resized)
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));

            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    menuState = !menuState;
                    if (menuState) {
                        music.pause();
                        ringtone.pause();
                        gunshotSound.pause();
                    }
                    else {
                        music.play();
                        ringtone.play();
                    }
                }
                if (event.key.code == sf::Keyboard::W) movingUp = true;
                if (event.key.code == sf::Keyboard::A) movingLeft = true;
                if (event.key.code == sf::Keyboard::S) movingDown = true;
                if (event.key.code == sf::Keyboard::D) movingRight = true;
            }

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::W) movingUp = false;
                if (event.key.code == sf::Keyboard::A) movingLeft = false;
                if (event.key.code == sf::Keyboard::S) movingDown = false;
                if (event.key.code == sf::Keyboard::D) movingRight = false;
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (hasGun && event.key.code == sf::Mouse::Left && bulletClock.getElapsedTime().asSeconds() > fireDelay) {

                    gunshotSound.play();
					sf::Texture& bulletTexture = assetManager.getTexture("bullet.png");
                    sf::Sprite bullet(bulletTexture);
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
            }
        }
        bool allEnemiesRemoved = true;

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
            while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window.close();
				}
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
					menuState = false;
					music.play();
					ringtone.play();
                    continue;
				}
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.key.code == sf::Mouse::Left) {
                        if (menuButtons[0].getShape().getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) 
                        {
                            window.close();
                        }

                        if (menuButtons[1].getShape().getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) 
                        {
                            window.clear();
                            window.draw(backgroundSprite);
                            hasGun = false;                           
                            gun.setPosition(generateRandomVector2f(window));
                            if (!hasGun) window.draw(gun);

                            playerSprite.setTexture(playerDefaultTexture);

                            spawnEnemies(enemies, playerSprite, gun, window);
                            for (int i = 0; i < 10; ++i) window.draw(enemies[i]);
                            window.draw(playerSprite);
                            bullets.clear();

                            angles.clear();
                            player.setHealth(100);
                            window.draw(healthText);
                            text.setString("Objective: Eradicate Tim Cheese (If you die, game over)");
                            text.setFillColor(sf::Color::Magenta);
                            window.draw(text);
                            music.play();
                            ringtone.play();
                            window.display();
                            menuState = false;
                            continue;
                        }
						if (menuButtons[2].getShape().getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))))
						{
							useHighQualityTextures = !useHighQualityTextures;
                            menuButtons[2].setText(std::string("HD Textures: ") + (useHighQualityTextures ? "On" : "Off"));

                            if (useHighQualityTextures == true) backgroundSprite.setTexture(backgroundTextureHQ, true);
                            if (useHighQualityTextures == false) backgroundSprite.setTexture(backgroundTexture, true);
                            backgroundSprite.setScale(
                                static_cast<float>(window.getSize().x) / backgroundSprite.getTexture()->getSize().x,
                                static_cast<float>(window.getSize().y) / backgroundSprite.getTexture()->getSize().y
                            );
						}
                    }
                }
            }
            continue;
        }

        #pragma region Controls
        float speed = 500.f;       
        if (movingUp) playerSprite.move(0.f, -speed * dt);
        if (movingDown) playerSprite.move(0.f, speed * dt);
        if (movingLeft) playerSprite.move(-speed * dt, 0.f);
        if (movingRight) playerSprite.move(speed * dt, 0.f);
        #pragma endregion

        healthText.setPosition(0.f, 60.f);
        healthText.setString("Health: " + std::to_string(player.getHealth()));
        healthText.setFillColor(player.getHealth() <= 50 ? sf::Color::Red : sf::Color::Green);
        
        if (!hasGun && playerSprite.getGlobalBounds().intersects(gun.getGlobalBounds())) {
            hasGun = true;
            playerSprite.setTexture(assetManager.getTexture("john_pork_with_gun.png"));
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
