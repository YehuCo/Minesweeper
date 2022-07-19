#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Tile.h"
#include "TextureManager.h"
#include "Random.h"
#include "Counter.h"
#include <vector>
#include <array>
using namespace std;

int main() {
    // board object, game start/over/victory tiles, debug/test 1/test 2/test 3 tiles
    Board board("boards/config.cfg");
    Tile face("images/face_happy.png");

    // set location of face
    face.SetLocation((board.width / 2) - 32, board.height - 100);

    // fill the tile vector with tiles, set mines to random tiles
    board.CreateTilesVector();

    // create the mine count sprites, and set their locations
    Counter counter1;
    Counter counter2;
    Counter counter3;
    Counter negativeSign;
    counter1.sprite.setPosition(21, board.height - 100);
    counter2.sprite.setPosition(42, board.height - 100);
    counter3.sprite.setPosition(63, board.height - 100);
    negativeSign.sprite.setPosition(0, board.height - 100);
    negativeSign.sprite.setTextureRect(sf::IntRect(210, 0, 21, 32));
    // set the counter to display the number of mines
    counter1.SetNumbers(board.numOfMines, counter2, counter3);

    // game loop
    while (board.screen.isOpen()) {

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (board.screen.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                board.screen.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                // get mouse position
                sf::Vector2i mousePos = sf::Mouse::getPosition(board.screen);
                // get the tile that is being clicked
                Tile& pressedTile = board.getPressedTile(mousePos);

                if (event.mouseButton.button == sf::Mouse::Right) {
                    if (pressedTile.clicked == false && board.gameOver == false) {
                        pressedTile.CheckIfFlagged();
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Left) {
                    if ((!pressedTile.clicked) && (!pressedTile.flagged) && (!board.gameOver) && (mousePos.x < board.width) && (mousePos.x > 0) && (mousePos.y < board.height - 100) && (mousePos.y > 0)) {
                        board.RevealTilesRecursive(pressedTile.row, pressedTile.column);
                        // check if the player revealed all valid tiles
                        board.CheckIfWin(face);
                        // check if the player revealed a mine
                        if (pressedTile.hasMine == true) {
                            board.gameOver = true;
                            face.ChangeTexture("images/face_lose.png");
                        }
                    }
                    else if ((mousePos.x < (face.coordinates.x + 64)) && (mousePos.x > face.coordinates.x)) {
                        if ((mousePos.y < (face.coordinates.y + 64)) && (mousePos.y > face.coordinates.y)) {
                            face.ChangeTexture("images/face_happy.png");
                            board.Reset();
                            counter1.SetNumbers(board.numOfMines, counter2, counter3);
                        }
                    }
                }
            }
        }

        // update the number to be displayed representing the number of mines
        counter1.UpdateNumber(board.numOfMines, board.numOfPlacedFlags, counter2, counter3);

        // clear the previous frame (background = white)
        board.screen.clear(sf::Color(255,255,255));

        // draw the tiles
        board.DrawTiles();

        // draw face and debug buttons
        board.screen.draw(face.sprite);
        board.screen.draw(counter1.sprite);
        board.screen.draw(counter2.sprite);
        board.screen.draw(counter3.sprite);
        // draw negative sign if number of flags placed exceeds number of mines
        if (board.numOfMines - board.numOfPlacedFlags < 0) {
            board.screen.draw(negativeSign.sprite);
        }
        
        // draw the numbers of the tiles
        board.DrawNumbers();

        // draw mines if player loses or if debug is clicked
        if (board.gameOver == true && board.tileCount - board.numOfRevealedTiles != board.numOfMines) {
            board.DrawMineTiles();
        }

        // draw the flagged tiles
        board.DrawFlaggedTiles();

        // display the new screen
        board.screen.display();

    }

    TextureManager::Clear();
    return 0;
}
