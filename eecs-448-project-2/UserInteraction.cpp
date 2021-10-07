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
  }
  else {
    cout << "Player 2 place your ships! [A-J][1-9]\n";
    player2->shipPlacement(numShips);
  }

  p1Turn = true;
  Board *player=player1;
  Board *opponent=player2;
  int currentPlayer=1;
  int opponentPlayer=2;
  for(; !opponent->hasLost(); p1Turn = !p1Turn)
  {

    currentPlayer = p1Turn ? 1 : 2;
    opponentPlayer = p1Turn ? 2 : 1;
    player = p1Turn ? player1 : player2;
    opponent = p1Turn ? player2 : player1;

    if(hasAi && currentPlayer == 2)
      opponent->firedAtByAi(aiDifficulty);
    else {
      system("clear");
      cout
        << "Player " << currentPlayer << "'s turn\n\n"
        << "\nYour Board:\n";

      player->viewBoard(false);

      cout << "\nOpponent's board:\n";
      opponent->viewBoard(true);

      cout << "\nWhere do you wish to fire?\n";

      do {
        opponent->fireAt();
        if(opponent->doScoreboard)
        {
            viewScores();
        }
      } while(opponent->doScoreboard);

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

void UserInteraction::viewScores()
{
    int p1SunkShips = player1->getSunkShips();
    int p2SunkShips = player2->getSunkShips();

    string gameState;
    if(p1SunkShips > p2SunkShips)
        gameState = "|        Player 1 is winning!         |";
    else if(p1SunkShips < p2SunkShips)
        gameState = "|        Player 2 is winning!         |";
    else
        gameState = "|           It is a draw...           |";

    system("clear");
    cout << "---------------------------------------\n"
         << gameState + "\n"
         << "|-------------------------------------|\n"
         << "|           |     P1     |     P2     |\n"
         << "|-------------------------------------|\n"
         << "| Ships Sunk|     " + to_string(p1SunkShips) + "      |     " + to_string(p2SunkShips) + "      |\n"
         << "|-------------------------------------|\n";

    for(int i = 0; i < numShips; i++)
    {
        cout << "|           | ";
        for(int j = 0; j <= i && i < 6; j++)
        {
            if(player1->getBoardState(i))
                cout << "X";
            else
                cout << "#";
        }
        for(int j = i; j < 6; j++)
        {
            cout << " ";
        }
        cout << "    | ";
        for(int j = 0; j <= i && i < 12; j++)
        {
            if(player2->getBoardState(i))
                cout << "X";
            else
                cout << "#";
        }
        for(int j = i; j < 6; j++)
        {
            cout << " ";
        }
        cout << "    |";
        cout << "\n";
    }

    cout << "|-------------------------------------|\n";
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
