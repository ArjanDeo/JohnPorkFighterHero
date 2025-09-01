#include "AssetManager.h"

AssetManager::AssetManager(const std::string& newDirectory) : assetsDirectory(newDirectory) {}

const std::map<AssetManager::DirectoriesEnum, std::string>& AssetManager::getDirectories() {
	static const std::map<DirectoriesEnum, std::string> directories = {
		{DirectoriesEnum::SOUND, "/sound/"},
		{DirectoriesEnum::IMAGE, "/images/"},
		{DirectoriesEnum::FONT, "/font/"},
	};
	return directories;
}


sf::SoundBuffer& AssetManager::getSoundBuffer(const std::string& soundName) {
	if (soundBuffers.find(soundName) == soundBuffers.end()) {
		loadSoundBuffer(soundName); // Load the sound if it doesn't exist
	}
	return soundBuffers[soundName]; // Return the sound
}
sf::Texture& AssetManager::getTexture(const std::string& textureName) {
	if (textures.find(textureName) == textures.end()) {
		loadTexture(textureName); // Load the texture if it doesn't exist
	}
	return textures[textureName]; // Return the texture
}
sf::Font& AssetManager::getFont(const std::string& fontName) {
	if (fonts.find(fontName) == fonts.end()) {
		loadFont(fontName); // Load the font if it doesn't exist
	}
	return fonts[fontName]; // Return the font
}

void AssetManager::loadSoundBuffer(const std::string& soundName) {
	// Initialize the SoundBuffer
	sf::SoundBuffer soundBuffer;

	if (!soundBuffer.loadFromFile(assetsDirectory + getDirectories().at(DirectoriesEnum::SOUND) + soundName)) {
		throw std::runtime_error("Failed to load sound: " + soundName);
	}
	soundBuffers[soundName] = soundBuffer; // Add the sound buffer to the map
}
void AssetManager::loadTexture(const std::string& textureName) {
	// Initialize the Texture
	sf::Texture texture;
	if (!texture.loadFromFile(assetsDirectory + getDirectories().at(DirectoriesEnum::IMAGE) + textureName)) {
		throw std::runtime_error("Failed to load texture: " + textureName);
	}
	textures[textureName] = texture; // Add the texture to the map
}
void AssetManager::loadFont(const std::string& fontName) {
	// Initialize the Font
	sf::Font font;
	if (!font.openFromFile(assetsDirectory + getDirectories().at(DirectoriesEnum::FONT) + fontName)) {
		throw std::runtime_error("Failed to load font: " + fontName);
	}
	fonts[fontName] = font; // Add the font to the map
}