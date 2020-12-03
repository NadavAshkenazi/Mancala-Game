/*-------------------------------------------------------------------------
  Include files:
-------------------------------------------------------------------------*/

#include "mancalaGame.c"

/*-------------------------------------------------------------------------
  The main program.
(the program allows two players to play mancala together and inform who won
or if its a draw.

rules:
=*=*=

The object of the game is to capture more stones than your opponent.

How to Play:
-=-=-=-=-=-
The Mancala board is made up of two rows of six holes and a home base hole,
for each player. six stones placed in each of the non-home holes.
The game begins with one player picking up all of the pieces in any one of
his holes.

Moving counter-clockwise, the player deposits one of the stones in each
hole until the stones run out.
If you run into your own home hole, deposit one stone in it. If you run
into your opponent's home hole, skip it. If the last stone you drop is in
your own home, you get an extra turn. If the last stone you drop is in an
empty hole on your side, you capture that stone and any stones in the hole
directly opposite. Always place all captured stones in your home.
The game ends when all six spaces on one side of the Mancala board are
empty.
The player who still has stones on his side of the board when the game ends
empties them into the opposite player's home. Count all the stones in each
home. The winner is the player with the most stones. an equal amount of
stones means a draw.)
 ------------------------------------------------------------------------*/
int main()
{
    int board[HOME2+CELL];
    reset_board(board);
    PrintWelcomeMessage();
    int end_game = play_mancala(board);
    if (end_game!= FAIL)
    {
        check_winner(board,end_game);
    }
    else
    {
        PrintScanfFailure();
    }

    return 0;
}