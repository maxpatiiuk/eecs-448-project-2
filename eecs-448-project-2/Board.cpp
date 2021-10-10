#include "Board.h"
#include "UserInteraction.h"

Board::Board()
{
  srand(time(nullptr));
  m_numShips = 0;
  m_shipsSunk = 0;
  m_aiRow = -1;
  m_aiCol = -1;
}

Board::~Board(){}

void Board::viewThisBoard()
{
  cout
    << "------------------------\n"
    << "|  A B C D E F G H I J |\n";

  for(int x = 0; x < ROWS; x++)
  {
    cout << '|' << x + 1 << ' ';

    for(int y = 0; y < COLS; y++) {
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
    viewThisBoard();

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
  viewThisBoard();
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
      if(validHorizontally && rand() % 2)
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
  while(true)
  {
    promptForCoordinate();

    if (m_grid[m_row][m_col].hitShip())
      break;

    cout << "This spot has already been hit. Try again.\n";
  }
  if (m_grid[m_row][m_col].isShip())
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
    if(m_aiRow == -1 || m_aiCol == -1){
      // Search for un-hit coordinate
      do {
        m_row=rand()%ROWS;
        m_col=rand()%COLS;
      } while(m_grid[m_row][m_col].hasBeenHit());
      if(m_grid[m_row][m_col].getChar() == SHIP){
        // if ship was hit, remember the position
        m_aiRow = m_row;
        m_aiCol = m_col;
      }
    } else {

      // determine if ship is horizontal or vertical
      bool hasShipAbove = isOnGrid(m_aiRow - 1, m_aiCol)
        && m_grid[m_aiRow - 1][m_aiCol].getChar() == HIT;
      bool hasShipBelow = isOnGrid(m_aiRow + 1, m_aiCol)
        && m_grid[m_aiRow + 1][m_aiCol].getChar() == HIT;
      bool hasShipToLeft = isOnGrid(m_aiRow, m_aiCol - 1)
        && m_grid[m_aiRow][m_aiCol - 1].getChar() == HIT;
      bool hasShipToRight = isOnGrid(m_aiRow, m_aiCol + 1)
        && m_grid[m_aiRow][m_aiCol + 1].getChar() == HIT;
      bool isVertical = hasShipBelow || hasShipAbove;
      bool isHorizontal = hasShipToLeft || hasShipToRight;

      // if can't determine ship's orientation, shot around it randomly
      if(!isHorizontal && !isVertical){
        vector<tuple<int, int>> neighbors;
        if(isOnGrid(m_aiRow + 1, m_aiCol)
          && !m_grid[m_aiRow + 1][m_aiCol].hasBeenHit())
          neighbors.push_back({m_aiRow + 1, m_aiCol});
        if(isOnGrid(m_aiRow - 1, m_aiCol)
          && !m_grid[m_aiRow - 1][m_aiCol].hasBeenHit())
          neighbors.push_back({m_aiRow - 1, m_aiCol});
        if(isOnGrid(m_aiRow, m_aiCol - 1)
          && !m_grid[m_aiRow][m_aiCol - 1].hasBeenHit())
          neighbors.push_back({m_aiRow, m_aiCol - 1});
        if(isOnGrid(m_aiRow, m_aiCol + 1)
          && !m_grid[m_aiRow][m_aiCol + 1].hasBeenHit())
          neighbors.push_back({m_aiRow , m_aiCol + 1});

        tuple<int, int> coords = neighbors.at(rand() % neighbors.size());
        m_row = get<0>(coords);
        m_col = get<1>(coords);
      }
      else {
        m_col = m_aiCol;
        m_row = m_aiRow;
        while(isOnGrid(m_row, m_col) && m_grid[m_row][m_col].getChar() == HIT){
          if(isVertical)
            m_row++;
          else
            m_col++;
        }
        if(!isOnGrid(m_row, m_col) || m_grid[m_row][m_col].getChar() == MISS){
          m_col = m_aiCol;
          m_row = m_aiRow;
          while(
            isOnGrid(m_row, m_col) && m_grid[m_row][m_col].getChar() == HIT
          ){
            if(isVertical)
              m_row--;
            else
              m_col--;
          }
        }
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
    m_aiRow = -1;
    m_aiCol = -1;
  }

  if (hasLost())
    cout << "Oh no, AI sunk all of your ships!!!\n";

}

void Board::promptForCoordinate()
{
  validInput = false;
  do
  {
    cout << "Choose a coordinate: ";
    cin >> userInput;

    if(userInput.length() == 2)
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
      for (int j = 0; j < shipSize; j++)
      {
        // If another ship takes up this space
        if (m_grid[m_row][m_col+j].isShip())
        {
          validInput = false;
          invalidCoordMessage = "Ships overlap.";
          break;
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
      for (int j = 0; j < shipSize; j++)
      {
        // If another ship takes up this space
        if (m_grid[m_row+j][m_col].isShip())
        {
          validInput = false;
          invalidCoordMessage = "Ships overlap.";
          break;
        }
      }
    }
  }
  if (!validInput){
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

  if(m_grid[row][col].isHorizontal())
  {
    temp = col;
    while (m_grid[row][temp].isShip())
    {
      if (!m_grid[row][temp].hasBeenHit())
        return false;

      temp--;

      if (!isOnGrid(row, temp))
        break;
    }

    temp = col;
    while (m_grid[row][temp].isShip())
    {
      if (!m_grid[row][temp].hasBeenHit())
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
      if (!m_grid[temp][col].hasBeenHit())
        return false;

      temp--;

      if (!isOnGrid(temp, col))
        break;
    }

    temp = row;
    while (m_grid[temp][col].isShip())
    {
      if (!m_grid[temp][col].hasBeenHit())
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

char Board::getShipGridChar(int x, int y)
{
    return m_grid[x][y].getChar();
}
