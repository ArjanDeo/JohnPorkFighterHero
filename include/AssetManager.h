#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
class AssetManager
{
public:
	AssetManager(const std::string& assetsDirectory);

	sf::SoundBuffer& getSoundBuffer(const std::string& soundName);
	sf::Texture& getTexture(const std::string& textureName);
	sf::Font& getFont(const std::string& fontName);
private:
	enum DirectoriesEnum {
		SOUND,
		IMAGE,
		FONT
	};
	static const std::map<DirectoriesEnum, std::string> Directories;
	std::string assetsDirectory;
	std::map<std::string, sf::SoundBuffer> soundBuffers;
	std::map<std::string, sf::Texture> textures;
	std::map<std::string, sf::Font> fonts;
	void loadSoundBuffer(const std::string& soundName);
	void loadTexture(const std::string& textureName);
	void loadFont(const std::string& fontName);
};
