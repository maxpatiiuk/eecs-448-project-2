#ifndef USERINTERACTION_H
#define USERINTERACTION_H

#include <cstdlib>
#include <limits>
#include <iostream>
#include "Board.h"

using namespace std;

class UserInteraction
{
private:

  // Points to player 1 board
  Board* player1;

  // Points to player 2 board
  Board* player2;

  // Is given by user and sets up the number of ships for placement
  int numShips;

  // Whether it is first player's turn
  bool isP1Turn;

  // Whether second player is an AI
  bool hasAi;

  // AI difficulty (between 1 and 3)
  int aiDifficulty;
  
    //player 1 special shot count
  int specShot1 = 1;
  //player 2 special shot count
  int specShot2 = 1;

public:
  /*
   * Initializes a board for each player
   * @param None
   */
  UserInteraction();

  // Destructor, deletes allocated memory
  ~UserInteraction();


  /*
   * Prompts user for an int within range
   * @param min
   * @param max
   */
  int promptForInt(int min, int max);

  /*
   * Run function calls playGame which contains the main game functionality
   * @param None
   */
  void playGame();

  void printArena();

  /*
   * Is called from main.cpp to start the game
   * @param None
   */
  void run();

};

#endif
