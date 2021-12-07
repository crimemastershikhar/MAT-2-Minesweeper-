#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <vector>
#include <algorithm>
#include <string>
#include <stdlib.h>
#define CLRSCR "\e[2J\e[H";
#define RED "\x1b[31m";
#define GREEN "\x1b[32m";
#define BLUE "\x1b[34m";
#define MAGENTA "\x1b[35m";
#define WHITE "\x1b[37m";
#define UNCOVERED_CELL '*';
#define BOMB 'B'
#define UP 'w'
#define DOWN 's'
#define RIGHT 'd'
#define LEFT 'a'
#define USER_POS '$'
#define PRESS_COMMAND 'p'
using namespace std;

int lines = 9, columns = 9, bombs = 10, is_game_finished = 0;
char board[100][100];                  //star used for bombs 
char user_board [100][100];
bool uncovered [100][100];
int user_line = 0;
int user_col = 0;
string Userinput;
bool is_inside(int line, int col)
{
  return 0<= line && line < lines && 0 <= col && col < columns;
}
void place_bombs()
{
  vector<pair<int, int> > positions;
  for(int line = 0; line < lines; line++)
    for(int col =0; col< columns;col++)
      positions.push_back(make_pair(line,col));            //Position wil start from 0
      random_shuffle(positions.begin(), positions.end());
      for(int index =0; index < bombs;++index)
      board[positions[index].first][positions[index].second] = 'B';
}
int compute_neighbours(int line, int col)
{
  int num_bombs = 0;
  for(int line_offset = -1; line_offset <= 1; ++line_offset)
    for(int col_offset = -1; col_offset <= 1; ++col_offset)  
    {
      int next_line = line + line_offset, next_col = col + col_offset;
        if(is_inside (next_line , next_col) && board[next_line][next_col] == BOMB)
        ++num_bombs;
    }
 return num_bombs;        
}

void init_Board(){
  for(int line = 0; line < lines; line++)
    for(int col =0; col< columns;col++)
      board[line][col] = user_board[line][col] = UNCOVERED_CELL;
    place_bombs();
  for(int line = 0; line < lines; line++)
    for(int col =0; col< columns;col++)    
    if(board[line][col] != 'B') 
      board [line][col] = compute_neighbours(line,col) + '0';  
}

void printBoard(){
  for(int line = 0; line < lines; line++)
  {
    for(int col =0; col< columns;col++)
    {
      cout<<user_board[line][col]<<" ";
    }
    cout<<endl;
  }
}
void start_game()
{
  user_line = user_col = 0;
  is_game_finished = 0;
  init_Board();
  user_board [user_line][user_col] = USER_POS;
}
void trigger_press (int line, int col)
{
  if (uncovered [line][col])
  return; 
  if (board[line][col] == BOMB)
  {
      is_game_finished = 1; 
      cout<<CLRSCR;
      cout<<RED;
      cout<<"                   YOU LOSE!!                     "<<endl;
      cout<<"                   GAME OVER                      "<<endl;
      cout<<WHITE; 
      cout<<"press 1 to RESTART "<<endl<<endl;
      cin>>Userinput;
        if(Userinput == "1")
        {
          cout<<CLRSCR;
          start_game();
        } 
  }else {
    uncovered[line][col] = 1;
    user_board[line][col] = board[line][col];
    if (user_board[line][col] == '0')
    {
      for(int line_offset = -1; line_offset <= 1; ++line_offset)
        for(int col_offset = -1; col_offset <= 1; ++col_offset)  
        {
          int next_line = line + line_offset, next_col = col + col_offset;
            if(is_inside (next_line , next_col))
            trigger_press(next_line, next_col);
        }   
    }
  }
}
void process_command(char command)
{
  int next_line = user_line, next_col = user_col;
  if(command == UP)
  {
    --next_line;
  }else if (command == DOWN)
  {
    ++next_line;
  }else if (command == LEFT)
  {
    --next_col;

  }else if (command == RIGHT)
  {
    ++next_col;
  }
  if(!is_inside(next_line, next_col))
  {
    return;
  }
  if(!uncovered[user_line][user_col])
  {
      user_board [user_line][user_col] = UNCOVERED_CELL;
  }
  else
    user_board[user_line][user_col] = board[user_line][user_col];  
  user_line = next_line;
  user_col = next_col;
  user_board [user_line][user_col] = USER_POS;  
  if(command == PRESS_COMMAND)
  {
    trigger_press(user_line, user_col);
  }
}
bool check_game_finished()
{
  int covered = 0;
  for(int line = 0; line < lines; line++)
     for(int col =0; col< columns;col++)  
     covered += 1-uncovered [line][col];
  if(covered == bombs)
  {
    cout<<"YOU WON THE GAME"<<endl;
    is_game_finished = 1;
    return true;
  }
  return false;
}
 void Intro()
 {
  cout<<RED;
  cout<<"***********************************************************************************************************************************"<<endl;
  cout<<"***********************************************************************************************************************************"<<endl;
  cout<<"***********************************************************************************************************************************"<<endl;
  cout<<"*********************                  WELCOME TO RETRO MINESWEEPER GAME          *******************************************"<<endl<<endl;
  cout<<"***********************************************************************************************************************************"<<endl;
  cout<<"***********************************************************************************************************************************"<<endl;
  cout<<"***********************************************************************************************************************************"<<endl;
  cout<<MAGENTA;
  cout<<"                                      RULES AND REGULATIONS"<<endl<<endl;
  cout<<"1. The board is divided into cells, with mines randomly distributed"<<endl<<"2. To Win, you need to open all the cells "<<endl<<"3. The number on a cell shows the number of mines adjacent to it"<<endl<<"4. Using the above information, you can deteremine cells that are safe, and cells that contain mines"<<"5. Interact, evolve and enjoy!!"<<endl<<endl;
  cout<<"----------------------------------------------------------------------------------------------------------------------------------------------------"<<endl<<endl;
  cout<<BLUE;
  cout<<"----------------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
  cout<<"                                         HOW TO PLAY       "<<endl;
  cout<<"1. The user can move across the matrix with (w - UP, s - DOWN, a - LEFT, d - RIGHT"<<endl<<"2. In each step, we will check for win or lose case "<<endl<<"The user can see a ($) sign which will show the current position of the user"<<endl<<endl;
  cout<<"----------------------------------------------------------------------------------------------------------------------------------------------------"<<endl<<endl;    
  cout<<WHITE;
  cout<<"press 1 to ENTER GAME "<<endl<<endl;
  cin>>Userinput;
    if(Userinput == "1")
    {
      cout<<CLRSCR;
      start_game();
    }
    else
    {
      cout<<"INVALID INPUT"<<endl;
      cout<<"RELOADING GAME.....WAIT"<<endl;
      sleep(2);
      cout<<CLRSCR;
      Intro();
    }
}
int main(){
  srand(time(0)); 
  Intro();
  while (!is_game_finished)
  {
    printBoard();
    char command;
    cout<<"User Input Command Required: ";
    cin>>command;
    process_command(command);
    is_game_finished = check_game_finished();
  }
  return 0;
}