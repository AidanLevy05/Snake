#include "conio.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>
using namespace std;

const int R = 17;
const int C = 17;
const int startR = 7;
const int startC = 3;

void welcome();
void clear();
void printColor(string, const string &);
void resetColor();
void initBoard(int board[][C], vector<pair<int, int>> &snake);
void printBoard(int board[][C]);
void play(int board[][C], vector<pair<int, int>> &snake);
void moveSnake(int &direction, int board[][C], vector<pair<int, int>> &snake,
               int &score);
void spawnApple(int board[][C]);
void updateScore(int &score);

int main() {
  // vars
  int board[R][C];
  vector<pair<int, int>> snake;

  initBoard(board, snake);

  // introduction
  welcome();

  // play
  play(board, snake);

  return 0;
}

void welcome() {
  clear();
  cout << "\n\nWelcome to SNAKE!\n\n" << endl;
  cout << "Press any key to start" << endl;
  _getch();
  clear();
}

void clear() { cout << "\033[2J\033[1;1H"; }

void printColor(string text, const string &color) {
  cout << color << text;
  resetColor();
}

void resetColor() { cout << "\033[0m"; }

void initBoard(int board[R][C], vector<pair<int, int>> &snake) {
  srand(time(0));
  for (int i = 0; i < R; i++)
    for (int j = 0; j < C; j++)
      board[i][j] = -1;

  // Initialize snake starting position
  snake.push_back({startR, startC});
  board[startR][startC] = 1;

  // Spawn the first apple
  spawnApple(board);
}

void printBoard(int board[R][C]) {
  // string colors
  const string RED = "\033[31m";
  const string GREEN = "\033[32m";
  const string BLUE = "\033[34m";

  for (int i = 0; i < R; i++) {
    for (int j = 0; j < C; j++) {
      // -1 == board piece
      // 0 == apple
      // 1 == snake

      if (board[i][j] == -1)
        printColor("[ ]", GREEN);
      else if (board[i][j] == 0)
        printColor("[*]", RED);
      else if (board[i][j] == 1)
        printColor(" O ", BLUE);
    }
    cout << endl;
  }
}

void play(int board[][C], vector<pair<int, int>> &snake) {
  int score = 1;
  int direction = 'D';
  while (true) {
    if (_kbhit()) {
      char move = _getch();
      if (move == 'q') {
        clear();
        break;
      }
      direction = move;
    }
    moveSnake(direction, board, snake, score);
    clear();
    printBoard(board);
    usleep(200000);
  }
}

void moveSnake(int &direction, int board[][C], vector<pair<int, int>> &snake,
               int &score) {
  // Get the current head position
  int headR = snake.front().first;
  int headC = snake.front().second;
  int newHeadR = headR, newHeadC = headC;

  // Move the head based on direction and check boundaries
  switch (direction) {
  case 'A':
  case 'a':
    newHeadC--;
    break;
  case 'D':
  case 'd':
    newHeadC++;
    break;
  case 'W':
  case 'w':
    newHeadR--;
    break;
  case 'S':
  case 's':
    newHeadR++;
    break;
  default:
    break;
  }

  // Check for collisions with walls or self
  if (newHeadR < 0 || newHeadR >= R || newHeadC < 0 || newHeadC >= C ||
      board[newHeadR][newHeadC] == 1) {
    cout << "Game Over!" << endl;
    updateScore(score);
    usleep(3000000);
    clear();
    exit(0);
  }

  // Check if new head position is an apple
  bool grow = false;
  if (board[newHeadR][newHeadC] == 0) {
    grow = true;
    score++;
    spawnApple(board);
  }

  // Update the board with the new head position
  board[newHeadR][newHeadC] = 1;
  snake.insert(snake.begin(), {newHeadR, newHeadC});

  if (!grow) {
    // Remove the tail from the board
    int tailR = snake.back().first;
    int tailC = snake.back().second;
    board[tailR][tailC] = -1;
    snake.pop_back();
  }
}

void spawnApple(int board[R][C]) {
  int appleR, appleC;
  do {
    appleR = rand() % R;
    appleC = rand() % C;
  } while (board[appleR][appleC] != -1);

  board[appleR][appleC] = 0;
}

void updateScore(int &score) {
  cout << "Your score: " << score << endl;

  // check new high score
  int prevScore;
  string prevName;

  ifstream inFile;
  inFile.open("highScore.txt");
  inFile >> prevScore >> prevName;
  inFile.close();

  // check which score is higher
  if (prevScore > score) {
    cout << "High score: " << prevScore << " set by " << prevName << endl;
  } else if (prevScore == score) {
    cout << "You tied with " << prevName << ", who has the same high score!"
         << endl;
  } else if (prevScore < score) {

    cout << "New high score!" << endl;
    cout << "You beat " << prevName << "'s score of " << prevScore << endl;

    string newName;
    cout << "Enter your username >> ";
    cin >> newName;
    cout << endl;

    ofstream outFile;
    outFile.open("highScore.txt");
    outFile << score << " " << newName;
    outFile.close();
  }
}