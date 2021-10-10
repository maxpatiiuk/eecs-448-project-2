#include "Board.h"
#include "UserInteraction.h"

Board::Board()
{
  srand(time(nullptr));
  m_numShips = 0;
  m_shipsSunk = 0;
}

Board::~Board(){}

void Board::viewBoard(bool asOpponent)
{
  cout
    << "------------------------\n"
    << "|  A B C D E F G H I J |\n";

  for(int x = 0; x < ROWS; x++)
  {
    cout << '|' << x + 1 << ' ';

    for(int y = 0; y < COLS; y++) {
      if(asOpponent && m_grid[x][y].getChar() == SHIP)
        // Hide enemy ships
        cout << BLANK;
      else
        cout << m_grid[x][y].getChar();
      cout << ' ';
    }

    cout << "|\n";
  }
  cout << "------------------------\n";
}

void Board::shipPlacement(int numShips)
{
  m_numShips = numShips;
  bool isHorizontal = true;
  char orientation;
  for (int i = 1; i <= m_numShips; i++)
  {
    // Show player their current board with ship placements
    viewBoard(false);

    // choose horizontal or vertical ship placement
    if (i != 1)
    {
      do
      {
        cout << "Place ship horizontally (H) or vertically (V)? ";
        cin >> orientation;
        if (orientation == 'h') orientation = HORIZONTAL;
        if (orientation == 'v') orientation = VERTICAL;

        if (orientation != HORIZONTAL && orientation != VERTICAL)
          cout << "Invalid input. Try again.";

      } while (orientation != HORIZONTAL && orientation != VERTICAL);

      isHorizontal = orientation == HORIZONTAL;
    }

    do {
      cout << "Choose the starting ";
      if(i != 1) isHorizontal ? cout << "leftmost " : cout << "topmost ";
      cout << "coordinate where you would like to place your ";
      isHorizontal ? cout << "1 x " << i : cout << i << " x 1";
      cout << " ship.\n";

      promptForCoordinate();

    } while(!checkValidShipPlacement(i, isHorizontal));

    if (isHorizontal)
      for (int j = 0; j < i; j++)
      {
        m_grid[m_row][m_col+j].placeShip();
        m_grid[m_row][m_col+j].setOrientation(isHorizontal);
        m_grid[m_row][m_col+j].setSize(i);
      }
    else
      for (int j = 0; j < i; j++)
      {
        m_grid[m_row+j][m_col].placeShip();
        m_grid[m_row+j][m_col].setOrientation(isHorizontal);
        m_grid[m_row+j][m_col].setSize(i);
      }

    system("clear");
  }

  cout << "Here is your final ship placement:\n";
  viewBoard(false);
  cout << "\nPress ENTER to Continue ";
  cin.ignore();
  cin.ignore();
  system("clear");
}

void Board::randomShipPlacement(int numShips){
  m_numShips = numShips;
  for(int shipSize=numShips; shipSize>=1; shipSize--)
    while(true){
      m_row = rand() % ROWS;
      m_col = rand() % COLS;

      bool validHorizontally = checkValidShipPlacement(shipSize, true);
      bool validVertically = checkValidShipPlacement(shipSize, false);

      if(!validHorizontally && !validVertically)
        continue;

      char orientation = validHorizontally ? HORIZONTAL : VERTICAL;
      if(validHorizontally && validHorizontally && rand() % 2)
        orientation = VERTICAL;

      if (orientation == HORIZONTAL)
        for (int j = 0; j < shipSize; j++)
        {
          m_grid[m_row][m_col+j].placeShip();
          m_grid[m_row][m_col+j].setOrientation(true);
        }
      else
        for (int j = 0; j < shipSize; j++)
        {
          m_grid[m_row+j][m_col].placeShip();
          m_grid[m_row+j][m_col].setOrientation(false);
        }

      break;
    }
}


void Board::fireAt()
{
    doScoreboard = false;
  while(true)
  {
    promptForCoordinate();
    if(doScoreboard){break;}

    if (m_grid[m_row][m_col].hitShip())
      break;

    cout << "This spot has already been hit. Try again.\n";
  }
  if(doScoreboard)
  {
      return;
  } else if (m_grid[m_row][m_col].isShip())
  {
    cout << "You hit an enemy ship!\n";

    if (isSunk(m_row, m_col))
    {
      cout << "You have sunk an enemy ship!\n";
      m_shipsSunk++;
    }

    if (hasLost())
      cout << "You have sunk all of your enemy's ships!\n";
  } else {
      cout << "\nYou missed.\n";
  }
}

void Board::firedAtByAi(int difficulty)
{
  if(difficulty == EASY)
    do {
      m_row=rand()%ROWS;
      m_col=rand()%COLS;
    } while(m_grid[m_row][m_col].hasBeenHit());

  else if(difficulty == MEDIUM){
    if(!isTargeting){
      do
      {
        originRow = rand() % ROWS;
        originCol = rand() % COLS;
      } while(m_grid[originRow][originCol].getChar() != BLANK);
      if(m_grid[originRow][originCol].hitShip())
      {
        isTargeting = true;
        m_targetRow = originRow;
        m_targetCol = originCol;
      }
    } else {
      int rowOffset = 0;
      int colOffset = 0;
      do{
        targetDirection = rand() % 4;
        bool isValidDir = false;
        while(!isValidDir){
          if(targetDirection == 0){
            rowOffset -= 1;
          } 
          else if(targetDirection == 1){
            rowOffset += 1;
          }
          else if(targetDirection == 2){
            colOffset -= 1;
          }
          else if(targetDirection == 3){
            colOffset += 1;
          }
        }
      }while(isOnGrid(m_targetRow + rowOffset, m_targetCol + colOffset) && m_grid[m_targetRow + rowOffset][m_targetCol = colOffset].getChar() != BLANK);
      m_grid[m_targetRow + rowOffset][m_targetCol + colOffset].hitShip();
      if(targetDirection == 0)
      {
        m_targetRow = m_targetRow + rowOffset;
      }
      if(targetDirection == 1)
      {
        m_targetRow = m_targetRow + rowOffset;
      }
      if(targetDirection == 2)
      {
        m_targetCol = m_targetCol + colOffset;
      }
      if(targetDirection == 3)
      {
        m_targetCol = m_targetCol + colOffset;
      }
    }
  }

  else if(difficulty == HARD)
    for(m_row=0; m_row<ROWS; m_row++)
      for(m_col=0; m_col<COLS; m_col++)
        if(m_grid[m_row][m_col].getChar() == SHIP)
          goto selected;
            selected:

  m_grid[m_row][m_col].hitShip();

  if(m_grid[m_row][m_col].getChar() == HIT)
    cout << "AI hit your ship!\n";
  else
    cout << "AI missed your ships!\n";

  if (isSunk(m_row, m_col))
  {
    cout << "AI sunk your ship!\n";
    m_shipsSunk++;
  }

  if (hasLost())
    cout << "Oh no, AI sunk all of your ships!!!\n";

}

void Board::promptForCoordinate()
{
  validInput = false;
  do
  {
    cout << "Choose a Coordinate or view Scoreboard [sb]: ";
    cin >> userInput;

    // check for scoreboard input flag
    if(userInput == "sb" || userInput == "sB" || userInput == "Sb" || userInput == "SB") {
        doScoreboard = true;
        return;
    } else if(userInput.length() == 2)
    {
      m_col = int(userInput.at(0));
      m_row = int(userInput.at(1));

      // Row number
      if (m_row >= 49 && m_row <=57)
      {
        m_row = m_row - 49;

        // Column letter, uppercase
        if (m_col >= 65 && m_col <= 74)
        {
          m_col = m_col - 65;
          validInput = true;
        }

        // Column letter, lowercase
        else if (m_col >= 97 && m_col <= 106)
        {
          m_col = m_col - 97;
          validInput = true;
        }
      }
    }

    if (!validInput)
    {
      cout << "ERROR: Input a valid coordinate, [A-J][1-9], for example, A1.\n";
    }

  } while(!validInput);
}


bool Board::checkValidShipPlacement(int shipSize, bool horizontal)
{
  validInput = true;

  if (horizontal)
  {
    if (m_col + shipSize > COLS)
    {
      validInput = false;
      invalidCoordMessage = "Ship goes off grid.";
    }

    else
    {
      if (
        (
          m_col > 0
          && m_grid[m_row][m_col-1].isShip()
        )
        || ( // Check if no ships to left or right
         m_col+shipSize < COLS
         && m_grid[m_row][m_col+shipSize].isShip()
        )
      )
      {
        validInput = false;
        invalidCoordMessage = "Cannot place ships adjacent to one another.";
      }

      for (int j = 0; j < shipSize; j++)
      {
        // If another ship takes up this space
        if (m_grid[m_row][m_col+j].isShip())
        {
          validInput = false;
          invalidCoordMessage = "Ships overlap.";
          break;
        }

        // Check no ships above
        if (m_row > 0)
        {
          if (m_grid[m_row-1][m_col+j].isShip())
          {
            validInput = false;
            invalidCoordMessage = "Cannot place ships adjacent to one another.";
            break;
          }
        }

        // Check no ships below
        if (m_row < ROWS-1)
        {
          if (m_grid[m_row+1][m_col+j].isShip())
          {
            validInput = false;
            invalidCoordMessage = "Cannot place ships adjacent to one another.";
            break;
          }
        }

      }
    }

  }

  else
  {
    if ((m_row + shipSize > ROWS))
    {
      validInput = false;
      invalidCoordMessage = "Ship goes off grid.";
    }

    else
    {
      if (
        (
          m_row > 0
          && m_grid[m_row-1][m_col].isShip()
        )
        || ( // check if no ships on top or below
          m_row+shipSize < m_row
          && m_grid[m_row+shipSize][m_col].isShip()
        )
      )
      {
        validInput = false;
        invalidCoordMessage = "Cannot place ships adjacent to one another.";
      }

      for (int j = 0; j < shipSize; j++)
      {
        // If another ship takes up this space
        if (m_grid[m_row+j][m_col].isShip())
        {
          validInput = false;
          invalidCoordMessage = "Ships overlap.";
          break;
        }

        // Check no ships left
        if (m_col > 0)
        {
          if (m_grid[m_row+j][m_col-1].isShip())
          {
            validInput = false;
            invalidCoordMessage = "Cannot place ships adjacent to one another.";
            break;
          }
        }

        // Check no ships right
        if (m_col < COLS-1)
        {
          if (m_grid[m_row+j][m_col+1].isShip())
          {
            validInput = false;
            invalidCoordMessage = "Cannot place ships adjacent to one another.";
            break;
          }
        }

      }
    }
  }

  if (validInput == false)
  {
    cout << "ERROR: " << invalidCoordMessage << "\n\n";
  }

  return validInput;

}

bool Board::isOnGrid(int row, int col)
{
  return (
    row >= 0
    && row < ROWS
    && col >= 0
    && col < COLS
  );
}

bool Board::isSunk(int row, int col)
{
  int temp;

  if(!m_grid[m_row][m_col].isShip())
    return false;

  if(m_grid[row][col].isHorizontal() == true)
  {
    temp = col;
    while (m_grid[row][temp].isShip())
    {
      if (m_grid[row][temp].hasBeenHit() == false)
        return false;

      temp--;

      if (!isOnGrid(row, temp))
        break;
    }

    temp = col;
    while (m_grid[row][temp].isShip())
    {
      if (m_grid[row][temp].hasBeenHit() == false)
        return false;

      temp++;

      if (!isOnGrid(row, temp))
        break;
    }

  }

  else
  {
    temp = row;
    while (m_grid[temp][col].isShip())
    {
      if (m_grid[temp][col].hasBeenHit() == false)
        return false;

      temp--;

      if (!isOnGrid(temp, col))
        break;
    }

    temp = row;
    while (m_grid[temp][col].isShip())
    {
      if (m_grid[temp][col].hasBeenHit() == false)
        return false;

      temp++;

      if (!isOnGrid(temp, col))
        break;
    }
  }

  return true;
}


bool Board::hasLost()
{
  return m_shipsSunk == m_numShips;
}



bool Board::getBoardState(int index)
{
    bool hitShipsBySize[6];
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            if(isSunk(i,j))
                hitShipsBySize[m_grid[i][j].getSize() - 1] = true;
        }
    }

    return hitShipsBySize[index];
}

