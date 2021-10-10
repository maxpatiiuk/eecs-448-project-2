#include "UserInteraction.h"
#include "Board.h"

UserInteraction::UserInteraction()
{
  player1 = new Board();
  player2 = new Board();
  numShips = 0;
}

UserInteraction::~UserInteraction()
{
  delete player1;
  delete player2;
}

int UserInteraction::promptForInt(int min, int max)
{
  // Prompt user for option
  int input = 0;

  while(true) {
    cin >> input;

    // Unset failBit
    cin.clear();

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if(cin.fail())
      cout << "ERROR: Your input did not seem to be a number. Try again.";

    else if(input < min || input > max)
      cout <<
        "ERROR: Please enter a number in range from "
        << min << " to " <<  max << ".\n";

    else
      return input;
  };
}

void UserInteraction::printArena()
{
    Board* playerOne = player1;
    Board* playerTwo = player2;
    int p1Sunk = playerOne->getSunkShips();
    int p2Sunk = playerTwo->getSunkShips();

    cout
        << "It is "
        << (isP1Turn ? "Player 1" : "Player 2")
        << "'s Turn\n"
        << "-----------------------------------------------\n"
        << "|       PLAYER         |        OPPONENT      |\n"
        << "|---------------------------------------------|\n"
        << "|  A B C D E F G H I J |  A B C D E F G H I J |\n";

    for(int x = 0; x < ROWS; x++)
    {
        cout << '|' << x + 1 << ' ';
        for(int y = 0; y < COLS; y++)
        {
            if(!isP1Turn && playerOne->getShipGridChar(x,y) == SHIP)
                // Hide enemy ships
                cout << BLANK;
            else
                cout << playerOne->getShipGridChar(x,y);
          cout << ' ';
        }

        cout << '|' << x + 1 << ' ';
        for(int y = 0; y < COLS; y++) {
            if(isP1Turn && playerTwo->getShipGridChar(x,y) == SHIP)
                // Hide enemy ships
                cout << BLANK;
            else
                cout << playerTwo->getShipGridChar(x,y);
            cout << ' ';
        }
        cout << "|\n";
    }

    cout << "-----------------------------------------------\n";

    if(isP1Turn)
    {
        cout
            << "You've sunk " + to_string(p2Sunk) + " of your enemy's ships\n"
            << (p2Sunk > p1Sunk ? "You are in the lead!\n" : (p2Sunk < p1Sunk ? "You are losing :(\n" : "It's a draw.\n"))
            << "This is a " + to_string(numShips) + "-ship game\n";
    } else {
        cout
            << "You've sunk " + to_string(playerOne->getSunkShips()) + " of your enemy's ships\n"
            << (p1Sunk > p2Sunk ? "You are in the lead!\n" : (p1Sunk < p2Sunk ? "You are losing :(\n" : "It's a draw.\n"))
            << "This is a " + to_string(numShips) + "-ship game\n";
    }
    cout << "-----------------------------------------------\n";
}


void UserInteraction::playGame()
{
  cout << "Input the number of ships you wish to play with (1-6): ";
  numShips = promptForInt(1, 6);
  system("clear");
  cout << "Player 1 place your ships! [A-J][1-9]\n";
  player1->shipPlacement(numShips);

  system("clear");
  cout << "Play against human (0) or AI (1)?\n";
  hasAi = promptForInt(0,1) == 1;

  system("clear");
  if(hasAi){
    cout
      << "Choose your artificial opponent:\n"
      << "1. Donkey Kong\n"
      << "2. IBM Deep Blue\n"
      << "3. Tesla Dojo\n";
    aiDifficulty = promptForInt(1,3);
    player2->randomShipPlacement(numShips);
  } else {
    cout << "Player 2 place your ships! [A-J][1-9]\n";
    player2->shipPlacement(numShips);
  }

  isP1Turn           = true;
  Board *player      = player1;
  Board *opponent    = player2;
  int currentPlayer  = 1;
  for(; !opponent->hasLost(); isP1Turn = !isP1Turn)
  {
    currentPlayer = isP1Turn ? 1 : 2;
    opponent = isP1Turn ? player2 : player1;

    if(hasAi && currentPlayer == 2)
      opponent->firedAtByAi(aiDifficulty);
    else {
      system("clear");
      printArena();
      opponent->fireAt();
      cin.ignore();
    }

    cout << "\nPress ENTER to Continue ";
    cin.ignore();

  }

  if(hasAi && currentPlayer == 2)
    cout << "AI Wins! Pesky humans die in fear\n\n";
  else
    cout << "\nPlayer " << currentPlayer << " wins!\n\n";
}

void UserInteraction::run()
{
  system("clear");
  cout
    << "Welcome to Battleship!\n"
    << "1. Start\n"
    << "0. Quit\n";

  if(promptForInt(0,1))
    playGame();
  else
    cout << "Goodbye!\n";
}
