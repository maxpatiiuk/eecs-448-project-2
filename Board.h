#ifndef BOARD_H
#define BOARD_H

#include "Ship.h"
using namespace std;

class Board
{
private:
  int numRows = 9;
  int numCols = 10;
  Ship m_grid[9][10]; // 2D array, of type Ship
  int m_numShips;

public:
  Board(); // initialize 9x10 board
  ~Board();
  void getNumShips(int numShips); // store number of ships in m_numShips
  void shipPlacement(int numShips); // place ships onto board
};

#endif
