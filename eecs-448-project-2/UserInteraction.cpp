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
    Board *player = isP1Turn ? player1 : player2;
    Board *opponent = isP1Turn ? player2 : player1;
    int currentPlayer = isP1Turn ? 1 : 2;

    cout
      << "It is Player " << currentPlayer << "'s Turn\n"
      << "-----------------------------------------------\n"
      << "|       PLAYER         |        OPPONENT      |\n"
      << "|---------------------------------------------|\n"
      << "|  A B C D E F G H I J |  A B C D E F G H I J |\n";

    for(int x = 0; x < ROWS; x++)
    {
      cout << '|' << x + 1 << ' ';
      for(int y = 0; y < COLS; y++)
        cout << player->getShipGridChar(x,y) << ' ';

      cout << '|' << x + 1 << ' ';
      for(int y = 0; y < COLS; y++) {
        if(opponent->getShipGridChar(x,y) == SHIP)
          // Hide enemy ships
          cout << BLANK;
        else
          cout << opponent->getShipGridChar(x,y);
        cout << ' ';
      }
      cout << "|\n";
    }

    int playerSunkShip = player->getSunkShips();
    int opponentSunkShip = opponent->getSunkShips();
    cout
      << "-----------------------------------------------\n"
      << "You've sunk " << playerSunkShip << " of your enemy's ships\n"
      << (playerSunkShip > opponentSunkShip
        ? "You are in the lead!\n"
        : playerSunkShip < opponentSunkShip
        ? "You are losing :(\n"
        : "It's a draw.\n")
      << "This is a " << numShips << "-ship game\n"
      << "-----------------------------------------------\n";
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
      int choice;
      system("clear");
      printArena();
      if(currentPlayer == 1){

        if(player->hasSpecialShot){
          cout << "1. Normal Shot\n";
          cout << "2. Special Shots\n";
          choice = promptForInt(1, 2);
        }
        else
          choice = 1;

        if(choice == 2){
          player->hasSpecialShot = false;
          opponent->fireAtThree();
        }
        else
          opponent->fireAt();
      }
    }

    cout << "\nPress ENTER to Continue ";
    cin.ignore();
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
