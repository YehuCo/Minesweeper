#pragma once
#include <iostream>
#include <array>
#include <SFML/Graphics.hpp>
using namespace std;

class Counter {
public:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::IntRect rect;

	Counter();
	void SetNumbers(int numMines, Counter& c2, Counter& c3);
	void UpdateNumber(int numMines, int numFlags, Counter& c2, Counter& c3);
};

// constructor
Counter::Counter() {
	rect = sf::IntRect(0, 0, 21, 32);
	texture.loadFromFile("images/digits.png");
	sprite.setTexture(texture);
	sprite.setTextureRect(rect);

}

// update the counters to display the number of mines
void Counter::SetNumbers(int numMines, Counter& c2, Counter& c3) {
	if (numMines >= 100) {
		int third = numMines % 10;
		c3.sprite.setTextureRect(sf::IntRect(21 * third, 0, 21, 32));
		numMines /= 10;
		int second = numMines % 10;
		c2.sprite.setTextureRect(sf::IntRect(21 * second, 0, 21, 32));
		int first = numMines / 10;
		this->sprite.setTextureRect(sf::IntRect(21 * first, 0, 21, 32));
	}
	else if (numMines >= 10) {
		int second = numMines % 10;
		c3.sprite.setTextureRect(sf::IntRect(21 * second, 0, 21, 32));
		int first = numMines / 10;
		c2.sprite.setTextureRect(sf::IntRect(21 * first, 0, 21, 32));
	}
	else {
		c3.sprite.setTextureRect(sf::IntRect(21 * numMines, 0, 21, 32));
	}
}

// change the displayed counters to show the number of mines
void Counter::UpdateNumber(int numMines, int numPlacedFlags, Counter& c2, Counter& c3) {
	int currentNum = numMines - numPlacedFlags;
	if (currentNum >= 100) {
		int third = currentNum % 10;
		c3.sprite.setTextureRect(sf::IntRect(21 * third, 0, 21, 32));
		currentNum /= 10;
		int second = currentNum % 10;
		c2.sprite.setTextureRect(sf::IntRect(21 * second, 0, 21, 32));
		int first = currentNum / 10;
		this->sprite.setTextureRect(sf::IntRect(21 * first, 0, 21, 32));
	}
	else if (currentNum >= 10) {
		int second = currentNum % 10;
		c3.sprite.setTextureRect(sf::IntRect(21 * second, 0, 21, 32));
		int first = currentNum / 10;
		c2.sprite.setTextureRect(sf::IntRect(21 * first, 0, 21, 32));
		this->sprite.setTextureRect(sf::IntRect(0, 0, 21, 32));
	}
	else if (currentNum >= 0) {
		c3.sprite.setTextureRect(sf::IntRect(21 * currentNum, 0, 21, 32));
		c2.sprite.setTextureRect(sf::IntRect(0, 0, 21, 32));
		this->sprite.setTextureRect(sf::IntRect(0, 0, 21, 32));
	}
	else if (currentNum < 0) {
		int currentPos = currentNum * -1;
		if (currentPos < 10) {
			c3.sprite.setTextureRect(sf::IntRect(21 * currentPos, 0, 21, 32));
			c2.sprite.setTextureRect(sf::IntRect(0, 0, 21, 32));
			this->sprite.setTextureRect(sf::IntRect(0, 0, 21, 32));
		}
		else if (currentPos < 100) {
			int second = currentPos % 10;
			c3.sprite.setTextureRect(sf::IntRect(21 * second, 0, 21, 32));
			int first = currentPos / 10;
			c2.sprite.setTextureRect(sf::IntRect(21 * first, 0, 21, 32));
			this->sprite.setTextureRect(sf::IntRect(0, 0, 21, 32));
		}
		else {
			int third = currentPos % 10;
			c3.sprite.setTextureRect(sf::IntRect(21 * third, 0, 21, 32));
			currentPos /= 10;
			int second = currentPos % 10;
			c2.sprite.setTextureRect(sf::IntRect(21 * second, 0, 21, 32));
			int first = currentPos / 10;
			this->sprite.setTextureRect(sf::IntRect(21 * first, 0, 21, 32));
		}
	}
}