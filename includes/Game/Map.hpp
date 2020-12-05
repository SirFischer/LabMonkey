#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include "Window.hpp"

class Map
{
private:
	sf::Texture				mSkyTexture;
	sf::Texture				mDirtTexture;
	sf::Texture				mGrassTexture;

	sf::Sprite				mSkySprite;
	sf::Sprite				mDirtSprite;
	sf::Sprite				mGrassSprite;

	std::vector<std::string>	mMapLines;

public:
	Map(std::string path);
	~Map();

	void Draw(Window *tWindow);
};