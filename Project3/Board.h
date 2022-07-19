#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Tile.h"
#include "Random.h"
using namespace std;

class Board {
public:
	unsigned int width;
	unsigned int height;
	int numOfMines;
	unsigned int tileCount;
	int numOfRevealedTiles;
	int numOfPlacedFlags;
	bool gameOver;
	sf::RenderWindow screen;
	vector<vector<Tile>> tiles;


	Board(string filename);
	~Board();
	void DrawTiles();
	void CreateTilesVector();
	Tile& getPressedTile(sf::Vector2i& mouse);
	void DrawFlaggedTiles();
	void AssignMines();
	void DrawMineTiles();
	void Reset();
	void FlagMines();
	void CheckNumOfMines();
	void DrawNumbers();
	void RevealTilesRecursive(int row, int column);
	void CheckIfWin(Tile& face);
};

// constructor
Board::Board(string filename) {

	// get width, height, and number of mines data from text file 
	ifstream file(filename);
	if (file.is_open()) {
		string line;
		int count = 1;

		while (getline(file, line, '\n')) {
			if (count == 1) {
				width = stoi(line);
			}
			else if (count == 2) {
				height = stoi(line);
			}
			else if (count == 3) {
				numOfMines = stoi(line);
			}
			count++;
		}
	}

	// set height and width
	tileCount = width * height;
	width *= 32;
	height = (height * 32) + 100;

	// initialize game trackers
	gameOver = false;
	numOfRevealedTiles = 0;
	numOfPlacedFlags = 0;
	// initialize screen
	screen.create(sf::VideoMode(width, height), "Minesweeper");
}

// destructor
Board::~Board() {}

// fill the tiles vector with Tile objects, and with each addition, update the position of the tile
void Board::CreateTilesVector() {
	for (unsigned int i = 0; i < height - 100; i += 32) {
		vector<Tile> row;
		for (unsigned int j = 0; j < width; j += 32) {
			Tile* tile = new Tile("images/tile_hidden.png");
			tile->coordinates.x = (float)j;
			tile->coordinates.y = (float)i;
			tile->sprite.setPosition(tile->coordinates);
			row.push_back(*tile);
		}
		tiles.push_back(row);
	}
	AssignMines();
	CheckNumOfMines();
}

// return the tile that was clicked
Tile& Board::getPressedTile(sf::Vector2i& mouse) {
	for (unsigned int i = 0; i < tiles.size(); i++) {
		for (unsigned int j = 0; j < tiles[i].size(); j++) {
			if ((tiles[i][j].coordinates.x < mouse.x) && ((tiles[i][j].coordinates.x + 32) > mouse.x)) {
				if ((tiles[i][j].coordinates.y < mouse.y) && ((tiles[i][j].coordinates.y + 32) > mouse.y)) {
					return tiles[i][j];
				}
			}
		}
	}
	Tile* noTile = new Tile("images/flag.png");
	noTile->row = tiles.size() + 5;
	noTile->column = tiles[0].size() + 5;
	return *noTile;
}

// assign mines to random tiles
void Board::AssignMines() {
	int numMines = numOfMines;
	while (numMines > 0) {
		int randTileX = Random::Int(0, width/32);
		int randTileY = Random::Int(0, (height - 100) / 32);
		for (int j = 0; j < tiles.size(); j++) {
			for (int k = 0; k < tiles[j].size(); k++) {
				if (tiles[j][k].coordinates.x == randTileX*32 && tiles[j][k].coordinates.y == randTileY*32) {
					if (tiles[j][k].hasMine == false) {
						tiles[j][k].hasMine = true;
						numMines--;
					}
				}
			}
		}
	}
}

// draw the tiles onto the screen
void Board::DrawTiles() {
	int numClicked = 0;
	for (unsigned int i = 0; i < tiles.size(); i++) {
		for (unsigned int j = 0; j < tiles[i].size(); j++) {
			screen.draw(tiles[i][j].sprite);
			tiles[i][j].row = i;
			tiles[i][j].column = j;
		}
	}
}

// if tile that has mine was clicked, draw all tiles that have mines
void Board::DrawMineTiles() {
	for (int i = 0; i < tiles.size(); i++) {
		for (int j = 0; j < tiles[i].size(); j++) {
			if (tiles[i][j].hasMine) {
				sf::Texture texture;
				texture.loadFromFile("images/mine.png");
				sf::Sprite mine(texture);

				mine.setPosition((float)tiles[i][j].coordinates.x, (float)tiles[i][j].coordinates.y);
				screen.draw(mine);
			}
		}
	}
}

// draw all the flagged tiles
void Board::DrawFlaggedTiles() {
	numOfPlacedFlags = 0;
	for (unsigned int i = 0; i < tiles.size(); i++) {
		for (unsigned int j = 0; j < tiles[i].size(); j++) {
			if (tiles[i][j].flagged == true) {
				numOfPlacedFlags++;
				sf::Texture texture;
				texture.loadFromFile("images/flag.png");
				sf::Sprite flag(texture);

				flag.setPosition((float)tiles[i][j].coordinates.x, (float)tiles[i][j].coordinates.y);
				screen.draw(flag);
			}
		}
	}
}

// draw all tile numbers
void Board::DrawNumbers() {
	for (int i = 0; i < tiles.size(); i++) {
		for (int j = 0; j < tiles[i].size(); j++) {
			if (tiles[i][j].clicked && tiles[i][j].numOfAdjacentMines != 0) {
				screen.draw(tiles[i][j].numSprite);
			}
		}
	}
}

// reset the board, reassign mines
void Board::Reset() {
	for (int i = 0; i < tiles.size(); i++) {
		for (int j = 0; j < tiles[i].size(); j++) {
			tiles[i][j].flagged = false;
			tiles[i][j].hasMine = false;
			tiles[i][j].clicked = false;
			tiles[i][j].numOfAdjacentMines = 0;
			tiles[i][j].ChangeTexture("images/tile_hidden.png");
			gameOver = false;
		}
	}
	this->AssignMines();
	numOfRevealedTiles = 0;

	CheckNumOfMines();
}

// flag all the mines when player wins the game
void Board::FlagMines() {
	for (int i = 0; i < tiles.size(); i++) {
		for (int j = 0; j < tiles[0].size(); j++) {
			if (tiles[i][j].hasMine == true) {
				tiles[i][j].flagged = true;
			}
		}
	}
}

// check the number of mines around each tile, and update the tile's numOfAdjacentMines attribute
void Board::CheckNumOfMines() {
	for (int i = 0; i < tiles.size(); i++) {
		for (int j = 0; j < tiles[i].size(); j++) {
			if (tiles[i][j].coordinates == tiles[0][0].coordinates) {
				if (tiles[i][j+1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i+1][j].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i+1][j+1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
			}
			else if (tiles[i][j].coordinates == tiles[0][tiles[0].size() - 1].coordinates) {
				if (tiles[i][j-1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i+1][j].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i+1][j-1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
			}
			else if (tiles[i][j].coordinates == tiles.at(tiles.size() -1).at(0).coordinates) {
				if (tiles[i-1][j].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i-1][j+1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i][j+1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
			}
			else if (tiles[i][j].coordinates == tiles[tiles.size() - 1][tiles[0].size() - 1].coordinates) {
				if (tiles[i][j-1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i-1][j-1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i-1][j].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
			}
			else if (tiles[i][j].coordinates.y == tiles[0][j].coordinates.y) {
				if (tiles[i][j - 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i + 1][j - 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i + 1][j].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i + 1][j + 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i][j + 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
			}
			else if (tiles[i][j].coordinates.y == tiles[tiles.size() - 1][j].coordinates.y) {
				if (tiles[i][j - 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i - 1][j - 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i - 1][j].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i - 1][j + 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i][j + 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
			}
			else if (tiles[i][j].coordinates.x == tiles[i][0].coordinates.x) {
				if (tiles[i - 1][j].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i - 1][j + 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i][j + 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i + 1][j + 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i + 1][j].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
			}
			else if (tiles[i][j].coordinates.x == tiles[i][tiles[0].size() - 1].coordinates.x) {
				if (tiles[i - 1][j].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i - 1][j - 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i][j - 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i + 1][j - 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i + 1][j].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
			}
			else {
				if (tiles[i - 1][j - 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i - 1][j].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i - 1][j + 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}

				if (tiles[i][j + 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i + 1][j + 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i + 1][j].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i + 1][j - 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
				if (tiles[i][j - 1].hasMine) {
					tiles[i][j].numOfAdjacentMines++;
				}
			}
			tiles[i][j].SetNumber();
		}
	}
}

// recursively reveal tiles around specified tile if they are not bombs
void Board::RevealTilesRecursive(int row, int column) {
	if ((row >= 0 && row < tiles.size()) && (column >= 0 && column < tiles[0].size())) {
		if (!tiles[row][column].hasMine) {
			if (tiles[row][column].numOfAdjacentMines != 0) {
				tiles[row][column].ChangeTexture("images/tile_revealed.png");
			}
			else if (tiles[row][column].numOfAdjacentMines == 0) {
				tiles[row][column].ChangeTexture("images/tile_revealed.png");
				if (tiles[row][column].coordinates == tiles[0][0].coordinates) {
					RevealTilesRecursive(row, column + 1);
					RevealTilesRecursive(row + 1, column + 1);
					RevealTilesRecursive(row + 1, column);
				}
				else if (tiles[row][column].coordinates == tiles[0][tiles[0].size() - 1].coordinates) {
					RevealTilesRecursive(row, column - 1);
					RevealTilesRecursive(row + 1, column - 1);
					RevealTilesRecursive(row + 1, column);
				}
				else if (tiles[row][column].coordinates == tiles[tiles.size() - 1][0].coordinates) {
					RevealTilesRecursive(row - 1, column);
					RevealTilesRecursive(row - 1, column + 1);
					RevealTilesRecursive(row, column + 1);
				}
				else if (tiles[row][column].coordinates == tiles[tiles.size() - 1][tiles[0].size() - 1].coordinates) {
					RevealTilesRecursive(row - 1, column);
					RevealTilesRecursive(row - 1, column - 1);
					RevealTilesRecursive(row, column - 1);
				}
				else if (tiles[row][column].coordinates.y == tiles[0][0].coordinates.y) {
					if (!tiles[row][column + 1].clicked) {
						RevealTilesRecursive(row, column + 1);
					}
					if (!tiles[row + 1][column + 1].clicked) {
						RevealTilesRecursive(row + 1, column + 1);
					}
					if (!tiles[row + 1][column].clicked) {
						RevealTilesRecursive(row + 1, column);
					}
					if (!tiles[row + 1][column - 1].clicked) {
						RevealTilesRecursive(row + 1, column - 1);
					}
					if (!tiles[row][column - 1].clicked) {
						RevealTilesRecursive(row, column - 1);
					}
				}
				else if (tiles[row][column].coordinates.y == tiles[tiles.size() - 1][0].coordinates.y) {
					if (!tiles[row][column + 1].clicked) {
						RevealTilesRecursive(row, column + 1);
					}
					if (!tiles[row - 1][column + 1].clicked) {
						RevealTilesRecursive(row - 1, column + 1);
					}
					if (!tiles[row - 1][column].clicked) {
						RevealTilesRecursive(row - 1, column);
					}
					if (!tiles[row - 1][column - 1].clicked) {
						RevealTilesRecursive(row - 1, column - 1);
					}
					if (!tiles[row][column - 1].clicked) {
						RevealTilesRecursive(row, column - 1);
					}
				}
				else if (tiles[row][column].coordinates.x == tiles[0][0].coordinates.x) {
					if (!tiles[row - 1][column].clicked) {
						RevealTilesRecursive(row - 1, column);
					}
					if (!tiles[row - 1][column + 1].clicked) {
						RevealTilesRecursive(row - 1, column + 1);
					}
					if (!tiles[row][column + 1].clicked) {
						RevealTilesRecursive(row, column + 1);
					}
					if (!tiles[row + 1][column + 1].clicked) {
						RevealTilesRecursive(row + 1, column + 1);
					}
					if (!tiles[row + 1][column].clicked) {
						RevealTilesRecursive(row + 1, column);
					}
				}
				else if (tiles[row][column].coordinates.x == tiles[0][tiles[0].size() - 1].coordinates.x) {
					if (!tiles[row - 1][column].clicked) {
						RevealTilesRecursive(row - 1, column);
					}
					if (!tiles[row - 1][column - 1].clicked) {
						RevealTilesRecursive(row - 1, column - 1);
					}
					if (!tiles[row][column - 1].clicked) {
						RevealTilesRecursive(row, column - 1);
					}
					if (!tiles[row + 1][column - 1].clicked) {
						RevealTilesRecursive(row + 1, column - 1);
					}
					if (!tiles[row + 1][column].clicked) {
						RevealTilesRecursive(row + 1, column);
					}
				}

				else {
					if (!tiles[row - 1][column - 1].clicked) {
						RevealTilesRecursive(row - 1, column - 1);
					}
					if (!tiles[row - 1][column].clicked) {
						RevealTilesRecursive(row - 1, column);
					}
					if (!tiles[row - 1][column + 1].clicked) {
						RevealTilesRecursive(row - 1, column + 1);
					}
					if (!tiles[row][column + 1].clicked) {
						RevealTilesRecursive(row, column + 1);
					}
					if (!tiles[row + 1][column + 1].clicked) {
						RevealTilesRecursive(row + 1, column + 1);
					}
					if (!tiles[row + 1][column].clicked) {
						RevealTilesRecursive(row + 1, column);
					}
					if (!tiles[row + 1][column - 1].clicked) {
						RevealTilesRecursive(row + 1, column - 1);
					}
					if (!tiles[row][column - 1].clicked) {
						RevealTilesRecursive(row, column - 1);
					}
				}

			}
		}
	}
}

// check if all valid tiles have been revealed, and if yes, make gameOver = true, flag all mines, and change the face's texture to win
void Board::CheckIfWin(Tile& face) {
	int numClicked = 0;
	for (int i = 0; i < tiles.size(); i++) {
		for (int j = 0; j < tiles[0].size(); j++) {
			if (tiles[i][j].clicked) {
				numClicked++;
			}
		}
	}
	numOfRevealedTiles = numClicked;

	if (tileCount - numOfRevealedTiles == numOfMines) {
		gameOver = true;
		FlagMines();
		face.ChangeTexture("images/face_win.png");
	}
}