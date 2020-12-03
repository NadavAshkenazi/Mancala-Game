/*-------------------------------------------------------------------------
  Include files:
-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/*=========================================================================
  Constants and definitions:
=========================================================================*/

/* put your #defines and typedefs here*/
#define PLAYER1 1
#define PLAYER2 2
#define NOBODY -1
#define HOME1 6
#define HOME2 13
#define WIDTH 2
#define STRT_NUM 6
#define FAIL -1
#define FIRST_CELL 0
#define CELL 1
#define EMPTY 0
/* GAME FUNCTIONS DECLARATIONS*/

void reset_board(int board[]);
int play_mancala(int board[]);
int scan_check(int board[],int turn);
bool player_indices(int player, int move);
int play_move(int move, int turn, int board[]);
int distribute_stones(int board[], int move, int turn);
int give_one_stone(int board[], int move, int turn);
int which_home(int player);
int facing_cell(int move);
int check_end_game(int board[]);
void check_winner(int board[],int end_game);
int switch_turn(int turn, int extra);

/* PRINTING FUNCTIONS DECLARATIONS*/
void PrintWelcomeMessage();
void PrintBoard(int board[]);
void PrintTurn(int player);
void PrintChooseCell();
void PrintScanfFailure();
void PrintInvalidValue();
void PrintEmptyCell();
void PrintWinningMessage(int winner);



/* GAME FUNCTIONS */

//resets the board to the starting state
void reset_board(int board[])
{
    for (int i=FIRST_CELL;i<=HOME2;i++)
    {
        switch(i)
        {
            case HOME1:
                board[i]=EMPTY;
                break;
            case HOME2:
                board[i]=EMPTY;
                break;
            default:
                board[i]=STRT_NUM;
        }
    }
}

/*A mother function that integrates all the step-functions of the game
 from the first input up until the point that one of the player's
 side is empty. the function returns the state at the end of the game:
  *if an input failed the program, the function will return FAIL value.
  *else, the function will return the number if the player who's side
   is empty.*/
int play_mancala(int board[])
{
    int end_game=EMPTY,turn = PLAYER1,move,extra=false;
    while(!end_game)
    {
        PrintBoard(board);
        PrintTurn(turn);
        PrintChooseCell();
        move=scan_check(board, turn);
        if (move!=FAIL) extra = play_move(move,turn,board);
        else return FAIL;
        end_game=check_end_game(board);
        turn=switch_turn(turn,extra);
    }
    return end_game;
}

/*checks the input given by the player.
    if it fails the system: prints the fail message and ends the game.
    if its invalid: prints the invalid message and scans again.
    if it points to an empty cell: prints an empty cell message and scans
    again.
    if the input doesn't fit any condition it counts as verified
    and returned.
    */
int scan_check(int board[],int turn)
{
    int move;
    while (true)
    {
        if((scanf("%d",&move)!=1))return FAIL;
        if((move<FIRST_CELL)||(HOME2<move)||(!player_indices(turn,move)))
            PrintInvalidValue();
        else if (board[move]==EMPTY) PrintEmptyCell();
        else break;
    }
    return move;
}

/*returns "true" if a given moves cell is within the range of a given
  player cells, otherwise returns "false".*/
bool player_indices(int player, int move)
{
    if (player == PLAYER1)
    {
        if ((move>=FIRST_CELL)&&(move<HOME1))
            return true;
        else return false;
    }
    else
    {
        if ((move>HOME1)&&(move<HOME2))
            return true;
        else return false;
    }
}

/*A function that integrates one move of a player.
      it receives a verified move an the distributes the stones (via
      a daughter-function) also:
      *if the last stone of the player's move fell in his home:
            the player will be marked as deserving another turn
      *if the last stone of the player's move fell in an empty cell of his:
            this cell and the cell that faces it in the board will be
            emptied into the player's home.
      returns if the player dese
    */
int play_move(int move, int turn, int board[])
{
    int extra=false;
    move=distribute_stones(board,move,turn);
    if ((move==HOME1)||(move==HOME2)) extra=true;
    else if((board[move]==CELL)&&(player_indices(turn,move)))
    {
        board[which_home(turn)]+=board[move]+board[facing_cell(move)];
        board[facing_cell(move)]=EMPTY;
        board[move]=EMPTY;
    }
    return extra;
}

/*takes all the stones from this move chosen cell and distributes this
  move stones from the chosen cell counter-clockwise.
  returns the index of the cell of the last stone drop*/
int distribute_stones(int board[], int move, int turn)
{
    int i=board[move];
    for(board[move]=EMPTY; i>EMPTY; i--)
    {
        move=give_one_stone(board, move, turn);
    }
    return move;
}

/*advances the destination index by 1 and then gives this index one
stone from the original chosen cell. returns the new destination
index*/
int give_one_stone(int board[], int move, int turn)
{
    if(turn==PLAYER1)
    {
        if(move==(HOME2-CELL))
        {
            board[FIRST_CELL]++;
            move = FIRST_CELL;
        }
        else
        {
            board[++move]++;
        }
    }
    else
    {
        if(move==(HOME1-CELL)) ++move;
        else if (move==(HOME2)) move=FIRST_CELL-CELL;
        board[++move]++;
    }
    return move;
}

//receives a players num' and returns the index of his home cell index
int which_home(int player)
{
    int home=HOME1;
    if (player==PLAYER2) home=HOME2;
    return home;
}

//receives a moves-cell and returns the facing cell in the board.
int facing_cell(int move)
{
    int facing_cell=move;
    if (player_indices(PLAYER1,move))
    {
        for(;move<HOME1;move++)
            facing_cell+=(CELL+CELL);
    }
    else
    {
        for(;move>HOME1;move--)
            facing_cell-=(CELL+CELL);
    }
    return facing_cell;
}

/*checks if one of the players entire side is empty and if so returns
  his number. otherwise, returns 'false'*/
int check_end_game(int board[])
{
    int end_game = PLAYER1;
    for (int i=FIRST_CELL; i<HOME1;i++)
    {
        if (board[i]!= EMPTY)
            end_game = false;
    }
    if (end_game==false)
    {
        end_game=PLAYER2;
        for (int i=FIRST_CELL; i<HOME1;i++)
        {
            if (board[HOME1+CELL+i]!= EMPTY)
                end_game = false;
        }
    }
    return end_game;
}

/*receives the number of the player who's side is empty and empties
  the rest of the board into his home. afterwards checks which of the
  home has more stones, and declares the owning player as the
  winner. if both homes have the same amount of stones, the function
  will declare it a draw*/
void check_winner(int board[],int end_game)
{
    if (end_game==PLAYER1)
    {
        for (int i=FIRST_CELL; i<HOME1;i++)
        {
            board[HOME1]+=board[HOME1+CELL+i];
            board[HOME1+CELL+i]=EMPTY;
        }
    }
    else
    {
        for (int i=FIRST_CELL; i<HOME1;i++)
        {
            board[HOME2]+=board[i];
            board[i]=EMPTY;
        }
    }
    PrintBoard(board);
    if (board[HOME1]>board[HOME2]) PrintWinningMessage(PLAYER1);
    else if (board[HOME1]<board[HOME2]) PrintWinningMessage(PLAYER2);
    else PrintWinningMessage(NOBODY);
}

/*checks if the current player deserves an extra turn and of not
 the function will switch the turn by returning the opposite player's
 number.*/
int switch_turn(int turn,int extra)
{
    if (extra==false)
    {
        if (turn==PLAYER1) turn=PLAYER2;
        else turn=PLAYER1;
    }
    return turn;
}

/* PRINTING FUNCTIONS */

//print the welcome message
void PrintWelcomeMessage()
{
    printf("Welcome to the game Mankala!\n");
}

//prints the board at a given message
void PrintBoard(int board[])
{

    printf("\n");
    printf("Player %*d indices:    ", WIDTH, PLAYER1);
    for(int i = HOME1; i >= 0; i--)
    {
        printf(" %*d  ", WIDTH, i);
    }
    printf("      \n");

    printf("                    |-----------------------------------------|\n");
    printf("                    |-----------------------------------------|\n");

    printf("                    ||    |");
    for(int i = HOME1-1; i >= 0; i--)
    {
        printf(" %*d |", WIDTH, board[i]);
    }
    printf("    ||\n");

    printf("                    || %*d |-----------------------------| %*d ||\n",
           WIDTH, board[HOME1], WIDTH, board[HOME2]);

    printf("                    ||    |");
    for(int i = HOME1+1; i < HOME2; i++)
    {
        printf(" %*d |", WIDTH, board[i]);
    }
    printf("    ||\n");

    printf("                    |-----------------------------------------|\n");
    printf("                    |-----------------------------------------|\n");

    printf("Player %*d indices:         ", WIDTH, PLAYER2);
    for(int i = HOME1+1; i <= HOME2; i++)
    {
        printf(" %*d  ", WIDTH, i);
    }
    printf("      \n\n");
}

//prints who's turn it is
void PrintTurn(int player)
{
    printf("Player %d turn.\n", player);
}

//ask the player to choose a non-empty cell
void PrintChooseCell()
{
    printf("Please choose one of your non-empty cells:\n");
}

//notifies that the input fail the program and forced an exit
void PrintScanfFailure()
{
    printf("Error: fail to read input! Exiting.\n");
}

/*notifies that the chosen cell is outside of the valid range
 of choices and asks to give another input*/
void PrintInvalidValue()
{
    printf("Error: invalid number of cell! please try again:\n");
}

/*notifies that the chosen cell is empty and therefor invalid and asks
 the player to give another input*/
void PrintEmptyCell()
{
    printf("Error: cell is empty! please try again:\n");
}
/*prints whether the game ended in a draw, and if not, prints the
  number of the winner*/
void PrintWinningMessage(int winner)
{
    if(winner == NOBODY)
    {
        printf("Draw!\n");
        return;
    }

    printf("The winner is player %d!\n", winner);
}
