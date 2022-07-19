#pragma once
#include <unordered_map>
#include <string>
#include "SFML/Graphics.hpp"
using namespace std;

class TextureManager {
	static std::unordered_map<std::string, sf::Texture> textures;
	static void LoadTexture(const char* fileName);
public:
	static sf::Texture& GetTexture(const char* name);
	static void Clear();
};

std::unordered_map<std::string, sf::Texture> TextureManager::textures;
void TextureManager::LoadTexture(const char* fileName) {
	string filePath = "images/";
	filePath += fileName;
	filePath += ".png";
	textures[fileName].loadFromFile(filePath);
}
sf::Texture& TextureManager::GetTexture(const char* name) {
	if (textures.find(name) == textures.end()) {
		LoadTexture(name);
	}
	return textures[name];
}
void TextureManager::Clear() { // clears the map elements
	textures.clear();
}
