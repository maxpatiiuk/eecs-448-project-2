#include "Ship.h"

Ship::Ship()
{
  m_char = BLANK;
  m_isShip = false;
  m_hasBeenHit = false;
  m_isHorizontal = true;
}

Ship::~Ship(){}

void Ship::placeShip()
{
  if (m_isShip == false)
  {
    m_isShip = true;
    m_char = SHIP;
  }
}

bool Ship::hitShip()
{
  if(m_hasBeenHit)
    return false;

  m_hasBeenHit = true;
  m_char = m_isShip ? HIT : MISS;
  return true;
}

void Ship::setOrientation(bool isHorizontal)
{
  m_isHorizontal = isHorizontal;
}

void Ship::setSize(int size)
{
    shipSize = size;
}

bool Ship::isHorizontal()
{
  return m_isHorizontal;
}

char Ship::getChar()
{
  return m_char;
}

bool Ship::hasBeenHit()
{
  return m_hasBeenHit;
}

bool Ship::isShip()
{
  return m_isShip;
}
