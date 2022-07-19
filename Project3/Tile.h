#pragma once
#include <iostream> 
#include <array>
using namespace std;

class Tile {
public:
	bool hasMine;
	bool flagged;
	bool clicked;
	int numOfAdjacentMines;
	int row;
	int column;
	sf::Vector2f coordinates;
	sf::Texture texture;
	sf::Texture numTexture;
	sf::Sprite sprite;
	sf::Sprite numSprite;

	Tile(string filename);
	~Tile();
	void SetLocation(float x, float y);
	void ChangeTexture(string filename);
	void CheckIfFlagged();
	void SetNumber();
};

// constructor
Tile::Tile(string filename) {
	texture.loadFromFile(filename);
	sprite.setTexture(texture);
	
	row = 0;
	column = 0;

	hasMine = false;
	flagged = false;
	clicked = false;
}

// destructor
Tile::~Tile() {}

// change the texture of the sprite
void Tile::ChangeTexture(string filename) {
	texture.loadFromFile(filename);
	sprite.setTexture(texture);

	if (filename == "images/tile_revealed.png") {
		this->clicked = true;
	}
}

// prepare sprite to display number of mines around tile
void Tile::SetNumber() {
	if (numOfAdjacentMines != 0) {
		numTexture.loadFromFile("images/number_" + to_string(numOfAdjacentMines) + ".png");
		numSprite.setTexture(numTexture);
		numSprite.setPosition(this->coordinates.x, this->coordinates.y);
	}
}

// set the location of the sprite
void Tile::SetLocation(float x, float y) {
	this->coordinates.x = x;
	this->coordinates.y = y;
	this->sprite.setPosition(this->coordinates);
}

// if the tile is flagged, flagged = true; otherwise false
void Tile::CheckIfFlagged() {
	if (this->flagged == false) {
		this->flagged = true;
	}
	else {
		this->flagged = false;
	}
}