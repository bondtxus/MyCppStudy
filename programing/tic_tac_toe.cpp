#include <iostream>
#include <ctime>
#include <typeinfo>
using namespace std;

class Player {
protected:
  char mark;			// Symbols set in the board
  int number;			// selected number (1-9)
public:
  // choose number (pure virtual function)
  virtual void select() = 0;
  char getMark() {     
    return mark;
  }
  int getNumber() {    
    return number;
  }
  // constructor
  Player(char mark) {	
    this->mark = mark;
  }
};

class User : public Player {
public:
  // choose a number
  void select() {		  
    int n;
    do {
      cout << "Your mark is ("<< mark << "). Please select a number = ";
      cin >> n;
    } while (n < 1 || n > 9);
    number = n;
  }
  // constructor
  User(char mark): Player(mark) {		
  }
};

class Computer : public Player {
public:
// choose a number
  void select() {		  
    number = rand() % 9 + 1;
  }
  // constructor
  Computer(char mark) : Player(mark) {  
  }
};

class Board {
private:
  char cell[3][3];		// 3×3 frame
public:
  void show();			  // Display the board
  bool setCheck(int number); // Check number availability
  void setCell(Player *player);	// Set a symbol in the board
  bool judge(Player *player);   // determine victory or defeat
  // constructor
  Board() {			      
    for (int v = 0; v < 3; v++) {
      for (int h = 0; h < 3; h++) {
        cell[v][h] = (v * 3 + h + 1) + '0';
      }
    }
  }
};


// Displaying the board
void Board::show() {
  cout << endl;
  for (int v = 0; v < 3; v++) {
    cout << "+-+-+-+" << endl;
    for (int h = 0; h < 3; h++) {
      cout << "|" << cell[v][h];
    }
    cout << "|" << endl;
  }
  cout << "+-+-+-+" << endl;
}

bool Board::setCheck(int number) {
  int v = (number - 1) / 3;
  int h = (number - 1) % 3;

  if (cell[v][h] >= '1' && cell[v][h] <= '9')
    return true;    // Returns true if set
  else
    return false;
}

// Set symbols in the board
void Board::setCell(Player *player) {
  int number = player->getNumber();
  int v = (number - 1) / 3;
  int h = (number - 1) % 3;
  cell[v][h] = player->getMark();
}

// determines victory or defeat
bool Board::judge(Player *player) {
  // Returns true if the player wins
  char mark = player->getMark();
  if (cell[0][0] == mark && cell[0][1] == mark && cell[0][2] == mark ||
    cell[1][0] == mark && cell[1][1] == mark && cell[1][2] == mark ||
    cell[2][0] == mark && cell[2][1] == mark && cell[2][2] == mark ||
    cell[0][0] == mark && cell[1][0] == mark && cell[2][0] == mark ||
    cell[0][1] == mark && cell[1][1] == mark && cell[2][1] == mark ||
    cell[0][2] == mark && cell[1][2] == mark && cell[2][2] == mark ||
    cell[0][0] == mark && cell[1][1] == mark && cell[2][2] == mark ||
    cell[2][0] == mark && cell[1][1] == mark && cell[0][2] == mark) {
    if (typeid(*player) == typeid(User)) 
      cout << endl << "Game over : " << " You win!" << endl;
    else
      cout << endl << "Game over : " << " You lose." << endl;

    return true;
  }

  // Returns true if it's a tie
  bool draw = true;
  for (int v = 0; v < 3 && draw; v++) {
    for (int h = 0; h < 3 && draw; h++) {
      if (cell[v][h] >= '1' && cell[v][h] <= '9') {
        draw = false;
      }
    }
  }
  if (draw) {
    cout << endl << "Game over : " << " Draw!" << endl;
    return true;
  }

  // Returns false if the winner is not determined.
  return false;
}



// main function
int main() {
  Player *player[2];		// 2 players (array of base classes)
  User user('o');		    // User (symbol is o)
  Computer computer('x');	// computer (symbol x)
  player[0] = &computer;	// First player (computer)
  player[1] = &user;		// Latest player (user)
  Board board;			    // Board object
  int turn = 0;			    // Order (switch between 0 and 1)

  // Initialize random numbers
  srand(time(0));

  // Repeat until the winner is decided
  while (true) {
    // Display the board
    board.show();

    // player chooses a number
    do {
      player[turn]->select();
    } while (!board.setCheck(player[turn]->getNumber()));
    if (typeid(*player[turn]) == typeid(Computer)) {
      cout << " Computer (" << player[turn]->getMark() << ") selected " << player[turn]->getNumber() << endl;
    }
    board.setCell(player[turn]);

    // determine victory or defeat
    if (board.judge(player[turn])) {
      // End the game when the winner is determined
      break;
    }

    // alternate players
    if (turn == 0) {
      turn = 1;
    }
    else {
      turn = 0;
    }
  }

  // Display the board
  board.show();

  // Before close window
  cout << "Enter any key to close window" << endl;
  int x;
  cin >> x;

  return 0;
}
