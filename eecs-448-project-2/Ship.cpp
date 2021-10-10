#include "Ship.h"

Ship::Ship()
{
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
  }
}

bool Ship::hitShip()
{
  if(m_hasBeenHit)
    return false;

  m_hasBeenHit = true;
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
  if(m_hasBeenHit)
    if(m_isShip)
      return HIT;
    else
      return MISS;
  else
    if(m_isShip)
      return SHIP;
    else
      return BLANK;
}

bool Ship::hasBeenHit()
{
  return m_hasBeenHit;
}

bool Ship::isShip()
{
  return m_isShip;
}
