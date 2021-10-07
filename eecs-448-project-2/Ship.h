#ifndef SHIP_H
#define SHIP_H

#include <iostream>

using namespace std;

#define BLANK ' '
#define MISS '.'
#define SHIP '#'
#define HIT '*'

class Ship
{
private:
  /*
   * Changes to BLANK when it is placed, changes to HIT when it is hit
   * @param None
   * @returns character value based on hit or placement
   */
  char m_char;

  int shipSize;

  /*
   * Checks to see whether or not the coordinate contains a ship
   * @param None
   * @return boolean value for coordinate
   */
  bool m_isShip;

  /*
   * Checks to see whether or not the ship has been hit
   * @param None
   * @return boolean value for ship status
   */
  bool m_hasBeenHit;

  /*
   * Sets a ships placement to either horizontal or vertical
   * @param None
   * @returns boolean value for horizontal or vertical
   */
  bool m_isHorizontal;


public:
  /*
   * Sets m_ship, m_hasBeenHit to false, m_isHorizontal to true, gridChar to MISS
   * @param None
   */
  Ship();

  /*
   * Deletes ship
   * @param None
   */
  ~Ship();

  /*
   * Checks if there is a ship, if not it makes is_ship true
   * @param None
   */
  void placeShip();

  /*
   * Marks sends hit to this coordinate
   * @param None
   */
  bool hitShip();

  /*
   * Checks to see if the ship has been sunk
   * @param None
   */
  void isSunk();

  /*
   * Changes state of m_isHorizontal
   * @param bool isHorizontal
   */
  void setOrientation(bool isHorizontal);

  void setSize(int size);

  int getSize()
  {
      return shipSize;
  }

  bool isHorizontal();

  /*
   * returns the char of the ship
   * @param none
   */
  char getChar();

  bool hasBeenHit();

  bool isShip();
};

#endif
