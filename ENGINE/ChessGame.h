#ifndef CHESSGAME_H_
#define CHESSGAME_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "ArrayList.h"

#define NUM_OF_PIECES 6
#define GRID 8
#define HISTORY_SIZE 6

enum {EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK};
#define PAWN_W 'p'
#define PAWN_B 'P'
#define BISHOP_W 'b'
#define BISHOP_B 'B'
#define ROOK_W 'r'
#define ROOK_B 'R'
#define KNIGHT_W 'n'
#define KNIGHT_B 'N'
#define QUEEN_W 'q'
#define QUEEN_B 'Q'
#define KING_W 'k'
#define KING_B 'K'

#define CH_EMPTY_ENTRY '_'
#define CH_PLAYER_1 'F' //(white player)
#define CH_PLAYER_2 'S'//(Black player)
#define CH_TIE 'T'

//represent the number of maxPossible direction of moves of each piece
#define KING_MOVES 8
#define QUEEN_MOVES 27
#define KNIGHT_MOVES 8
#define ROOK_MOVES 14
#define BISHOP_MOVES 13
#define PAWN_MOVES 4

// represent the number of each kind of piece
#define PAWN_NUM 8
#define B_R_Kn_NUM 2
#define K_Q_NUM 1

//error checking flag
enum{
    POSITION_INVALID = 1,
    ILLEGAL_FIGURE,
    ILLEGAL_MOVE,
    ALLOCATION_ERROR
};
//error checking status
typedef enum CH_STATUS{
    CHECK,
    CHECKMATE,
    TIE,
    REGULAR
}CH_STATUS;



//represent status
enum{
    CHECK_REP,
    CHECKMATE_REP,
    TIE_REP,
    REG_REP
};

//Piece Index
enum{
    PAWN_INDEX,
    KNIGHT_INDEX,
    BISHOP_INDEX,
    ROOK_INDEX,
    QUEEN_INDEX,
    KING_INDEX
};

//default arguments
#define DEFAULT_MODE 1
#define DEFAULT_COLOR 1
#define DEFAULT_DIFFICULTY 3
#define TWO_PLAYER_MODE 2

//represent moves to col ans row
enum{
    START_ROW_PLACE,
    START_COL_PLACE,
    DESTINATION_ROW_PLACE,
    DESTINATION_COL_PLACE
};

//represent whole game data
typedef struct ch_game{
    char gameBoard[GRID][GRID];
    CH_STATUS game_status;
    int white[NUM_OF_PIECES];
    int black[NUM_OF_PIECES];
    char currentPlayer;
    SPArrayList* undo_hist;
    int white_king[2];
    int black_king[2];
    int mode;
    int difficulty;
    int user_color;// 1 for white and 0 for black
    int piece_promotion[4];
    int mv_cnt;
}CH_Game;

//for error detection in game function
typedef enum ch_game_message_t{
    CH_INVALID_MOVE,
    CH_INVALID_ARGUMENT,
    CH_NO_HISTORY,
    CH_SUCCESS
}CH_GAME_MESSAGE;
/**
 * Creates a new game with a specified history size. The history size is a
 * parameter which specifies the number of previous moves to store. If the number
 * of moves played so far exceeds this parameter, then first moves stored will
 * be discarded in order for new moves to be stored.
 *
 * @param historySize - The total number of moves to undo,
 *                      a player can undo at most historySizeMoves turns.
 * @param diff - game difficulty number between 1 and 5
 * @param color - user's colour (black or white - 0 or 1)
 * @param mode -game mode - one or two players (zero or one)
 *
 * @return
 * NULL if either a memory allocation failure occurs or historySize <= 0.
 * Otherwise, a new game instant is returned.
 */
extern CH_Game* gameCreate(int historySize,int diff,int color, int mode);
/**
 * Initializes a new game board with all the chess figures in there initial places
 *
 * @param game - the game which need to be initialize
 *
 * @return - no return - void function that changes the game board
 */
extern void boardInitialize(CH_Game *game);
/**
 * Initializes the game default start arguments
 *
 * @param game - the game which need to be initialize
 *
 * @return - no return - void function that changes struct fields
 */
extern void argInitialize(CH_Game* game);
/**
 *	Creates a copy of a given game.
 *	The new copy has the same fields and parameters as the src game
 *
 *	@param src - the source game which will be copied
 *
 *	@return
 *	NULL if either src is NULL or a memory allocation failure occurred.
 *	Otherwise, an new copy of the source game is returned.
 *
 */
extern CH_Game* gameCopy(CH_Game* src);
/**
 * copies specific game params
 * @param src - the game to be copied
 * @param gamecopy - the game to which the data is copied
 *
 * @return
 * no return - void function
 */
extern void copyParameters(CH_Game* src,CH_Game* gamecopy);
/**
 * Returns the current player of the specified game.
 * @param src - the source game
 * @return
 * CH_PLAYER_1 - if it's player one's turn
 * CH_PLAYER_2 - if it's player two's turn
 * CH_TIE      - otherwise
 */
extern char getCurrentPlayer(CH_Game* src);
/**
 * Frees all memory allocation associated with a given game. If src==NULL
 * the function does nothing.
 *
 * @param src - the source game
 */
extern void gameDestroy(CH_Game* src);
/**
 * On success, the function prints the board game. If an error occurs, then the
 * function does nothing. The characters represent different figures.
 * white player(player 1) represented by small letters
 * black player(player 2) represented by capital letters
 *
 * @param src - the target game
 *
 * @return
 * CH_INVALID_ARGUMENT - if src==NULL
 * CH_GAME_SUCCESS - otherwise
 *
 */
extern CH_GAME_MESSAGE gamePrintBoard(CH_Game* src);
/**
 * Sets the next move in a given game by specifying x and y move coordinates. The
 * moves are 0-based and in the range [0,GRID -1].
 *
 * @param src - The target game
 * @param move - The move the the player wants to perform (from x,y to x,y)
 * @return
 * CH_INVALID_ARGUMENT - if src is NULL or move is out-of-range
 * CH_INVALID_MOVE - if the given move is not a valid one (many reasons - overlaps, not correct coordinates)
 * CH_SUCCESS - otherwise
 */
extern int setMove(CH_Game* src, int* move);
/**
* does the move given-updates the game board, the pieces vector array and kings' positions
* switchs player, updates the history array list
*
* @param src - the source game , col-valid column
* @assertion error if src == null.
*/
extern void doMove(CH_Game* src, elem* element);
/**
 * Makes an element that represents a move with all relevant fields
 *
 * @param src - The target game
 * @param move - The move the player wants to perform
 * @return
 * flag - for each move state
 */
extern elem* transfer(CH_Game* src ,int* move);
/**
 * return an enum that epresents the game's status by status name and not by int number
 *
 * @param st - enum that represents a game status
 * @return
 * 0             - if st==CHECK
 * 1             - if st==CHECKMATE
 * 2             - if st == TIE
 * 3             - otherwise
 */
extern int statusToInt(CH_STATUS st);
/**
 * return an enum that epresents the game's status by status name and not by int number
 *
 * @param n - number that represents a game status
 * @return
 * CHECK               - if n == 0
 * CHECKMATE           - if n == 1
 * TIE                 - if n == 2
 * REGULAR             - otherwise
 */
extern CH_STATUS intToStatus(int n);
/**
 * Checks if the figure in the current move is current player's figure
 *
 * @param src - The target game
 * @param row, col
 * @return
 * true - if is valid
 * false - if is not valid
 */
extern bool playerFig(CH_Game* src, int row, int col);
/**
 * Checks which piece is making a move and checks if the direction of the move is valid
 *
 * @param src - The target game
 * @param element - struct that represents a move that the player wants to do
 * @return
 * true - if the move is valid
 * false - if the move is not valid
 */
extern bool figureMove(CH_Game* src, elem* element);
/**
 * Checks which piece is making a move and checks if the move doesn't overlaps other pieces
 *
 * @param src - The target game
 * @param element - struct that represents a move that the player wants to do
 * @return
 * true - if the move doesn't overlaps - returns true if the dst cell has an opponent's piece
 * false - if the move overlaps
 */
extern bool noOverlap(CH_Game* src, elem* element);
/**
 * Checks if a move can be done
 *
 * @param src - The source game
 * @param element - Represents a move with all relevant fields that the player wants to perform
 * @return
 * flag for each move set state
 */
extern int isValidMove(CH_Game* src , elem* element);
/**
 * Checks if the move is a valid move for a pawn
 *
 * @param element - struct that represents a move that the player wants to do
 * @return
 * true - if the move is valid pawn move
 * false - if the move is not valid pawn move
 */
extern bool pawnMove(CH_Game* src, elem* element);
/**
 * Checks if the move is a valid move for a bishop
 *
 * @param element - struct that represents a move that the player wants to do
 * @return
 * true - if the move is valid bishop move
 * false - if the move is not valid bishop move
 */
extern bool bishopMove(elem* element);
/**
 * Checks if the move is a valid move for a rook
 *
 * @param element - struct that represents a move that the player wants to do
 * @return
 * true - if the move is valid rook move
 * false - if the move is not valid rook move
 */
extern bool rookMove(elem* element);
/**
 * Checks if the move is a valid move for a knight
 *
 * @param element - struct that represents a move that the player wants to do
 * @return
 * true - if the move is valid knight move
 * false - if the move is not valid knight move
 */
extern bool knightMove(elem* element);
/**
 * Checks if the move is a valid move for a queen
 *
 * @param element - struct that represents a move that the player wants to do
 * @return
 * true - if the move is valid queen move
 * false - if the move is not valid queen move
 */
extern bool queenMove(elem* element);
/**
 * Checks if the move is a valid move for a king
 *
 * @param element - struct that represents a move that the player wants to do
 * @return
 * true - if the move is valid king move
 * false - if the move is not valid king move
 */
extern bool kingMove(elem* element);
/**
 * Checks if the move is diagonal
 *
 * @param element - struct that represents a move that the player wants to do
 * @return
 * true - if the move is diagonal
 * false - o.w
 */
extern bool isDiagonal(elem* element);
/**
 * Checks if the dst cell has an opponent's piece
 *
 * @param src - The target game
 * @param row - the row in which the piece is
 * @parm col - the col in which the piece is
 * @return
 * true - if the dst cell has an opponent's piece
 * false - o.w
 */
extern bool isOppPiece(CH_Game* src, int row, int col);
/**
 * Checks if the given cell represented by row and col is empty
 *
 * @param src - The target game
 * @param row - represents the row of the cell
 * @param col - represents the col of the cell
 * @return
 * true - if the cell is empty
 * false - o.w
 */
extern bool isCellEmpty(CH_Game* src, int row, int col);
/**
 * Checks if the given pawn move doesn't overlaps pieces
 *
 * @param src - The target game
 * @param element - struct that represents a move that the player wants to do
 * @param check - set to true if the player can stay at place, due to check potintiel.
 * @return
 * true - if the move is fine- the overlap is legal - opponent's piece in dst cell or there is no overlap at all
 * false - o.w
 */
extern bool pawnOverlap(CH_Game* src,elem* element);
/**
 * Checks if the given bishop move doesn't overlaps pieces
 *
 * @param src - The target game
 * @param element - struct that represents a move that the player wants to do
 * @return
 * true - if the move is fine- the overlap is legal - opponent's piece in dst cell or there is no overlap at all
 * false - o.w
 */
extern bool bishopOverlap(CH_Game* src,elem* element);
/**
 * Checks if the given rook move doesn't overlaps pieces
 *
 * @param src - The target game
 * @param element - struct that represents a move that the player wants to do
 * @return
 * true - if the move is fine- the overlap is legal - opponent's piece in dst cell or there is no overlap at all
 * false - o.w
 */
extern bool rookOverlap(CH_Game* src,elem* element);
/**
 * Checks if the given queen move doesn't overlaps pieces
 *
 * @param src - The target game
 * @param element - struct that represents a move that the player wants to do
 * @return
 * true - if the move is fine- the overlap is legal - opponent's piece in dst cell or there is no overlap at all
 * false - o.w
 */
extern bool queenOverlap(CH_Game* src,elem* element);
/**
 * Checks if the given king or knight move doesn't overlaps pieces
 *
 * @param src - The target game
 * @param element - struct that represents a move that the player wants to do
 * @return
 * true - if the move is fine- the overlap is legal - opponent's piece in dst cell or there is no overlap at all
 * false - o.w
 */
extern bool kingOrKnightOverlap(CH_Game* src,elem* element);
/**
 * Undoes the previous move - returns to boarrd the captured figure, if needed, and switchs
 * player's turn. Also, updates the kings' positions' to previous one.
 * If the user invoked this command more than historySize times
 * in a row then an error occurs.
 *
 * @param src - The source game
 * @return
 * CH_INVALID_ARGUMENT - if src == NULL
 * CH_NO_HISTORY       - if the user invoked this function more then
 *                       historySize in a row.
 * CH_SUCCESS          - on success. The last disc that was put on the
 *                       board is removed and the current player is changed
 */
extern CH_GAME_MESSAGE undoPrevMove(CH_Game* src);
/**
* Checks if there's a winner in the specified game status. The function returns either
* CH_PLAYER_2 or CH_PLAYER_1 in case there's a winner, where
* the value returned is the symbol of the winner. If the game is over and there's a tie
* then the value CH_TIE is returned. in any other case the null characters
* is returned.
* @param src - the source game
* @return
* CH_PLAYER_1 - if player 1 won
* CH_PLAYER_2 - if player 2 won
* CH_TIE - If the game is over and there's a tie
* null character - otherwise
*/
extern char checkWinner(CH_Game* src);
/**
* switchs the current player
*
* @param src - the source game
* @assertion error if src == null.
*/
extern void switchPlayer(CH_Game* src);
/**
* Checks if the king of the current player is under attack - can be captured by some opp piece
*
* @param src - the source game
* @param row - the piece row
* @param col - the piece col
* @return
* true - if the king can be captured
* false - otherwise
*/
extern bool pieceUnderAttack(CH_Game* src,int row, int col);
/**
* Updates the kings' positions
*
* @param src - the source game
* @param fig - a char that represents the figure
* @param row - the row in which the figure is now
* @param col - the col in which the figure is now
*
* @assertion error if src == null.
*/
extern void kingUpdate(CH_Game* src, char fig, int row, int col);
/**
* Updates the game's status and returns it
*
* @param src - the source game
*
* @return
* CHECK               - if the game status is now check
* CHECKMATE           - if the game status is now checkmate
* TIE                 - if the game status is now tie
* REGULAR             - otherwise
*/
extern CH_STATUS updateStatus(CH_Game* src);
/**
* Computes all possible moves of a king in the position given. saves all possible moves to an array
*
* @param src - the source game
* @param row - the row in which the figure is now
* @param col - the col in which the figure is now
*
* @return
* NULL - if src==NULL or there is an allocation problem
* a 2-dimensional array with all the possible moves of the king - an impossible move is represented by -1,-1 array cell - otherwise
*/
extern int** possibleKingMoves(CH_Game* src,int row, int col);
/**
* Computes all possible moves of a queen in the position given. saves all possible moves to an array
*
* @param src - the source game
* @param row - the row in which the figure is now
* @param col - the col in which the figure is now
*
* @return
* NULL - if src==NULL or there is an allocation problem
* a 2-dimensional array with all the possible moves of the king - an impossible move is represented by -1,-1 array cell - otherwise
*/
extern int** possibleQueenMoves(CH_Game* src, int row, int col);
/**
* Computes all possible moves of a knight in the position given. saves all possible moves to an array
*
* @param src - the source game
* @param row - the row in which the figure is now
* @param col - the col in which the figure is now
*
* @return
* NULL - if src==NULL or there is an allocation problem
* a 2-dimensional array with all the possible moves of the king - an impossible move is represented by -1,-1 array cell - otherwise
*/
extern int** possibleKnightMoves(CH_Game* src, int row, int col);
/**
* Computes all possible moves of a rook in the position given. saves all possible moves to an array
*
* @param src - the source game
* @param row - the row in which the figure is now
* @param col - the col in which the figure is now
*
* @return
* NULL - if src==NULL or there is an allocation problem
* a 2-dimensional array with all the possible moves of the king - an impossible move is represented by -1,-1 array cell - otherwise
*/
extern int** possibleRookMoves(CH_Game* src, int row, int col);
/**
* Computes all possible moves of a bishop in the position given. saves all possible moves to an array
*
* @param src - the source game
* @param row - the row in which the figure is now
* @param col - the col in which the figure is now
*
* @return
* NULL - if src==NULL or there is an allocation problem
* a 2-dimensional array with all the possible moves of the king - an impossible move is represented by -1,-1 array cell - otherwise
*/
extern int** possibleBishopMoves(CH_Game* src,int row, int col);
/**
* Computes all possible moves of a pawn in the position given. saves all possible moves to an array
*
* @param src - the source game
* @param row - the row in which the figure is now
* @param col - the col in which the figure is now
*
* @return
* NULL - if src==NULL or there is an allocation problem
* a 2-dimensional array with all the possible moves of the king - an impossible move is represented by -1,-1 array cell - otherwise
*/
extern int** possiblePawnMoves(CH_Game* src,int row, int col);
/**
* If the move is valid - Adds a coordinates that represent a move to the relevant move array and returns true
*
* @param src - the source game
* @param row - the row in which the king is
* @param col - the col in which the king is
* @param i - the row in which the figure is
* @param j - the col in which the figure is
* @param arr - the 2-dim array to which the move should be added
* @param index - the index in the array where the move should be added
*
* @return
* NULL - if src==NULL or arr==NULL
* true - if the move is valid
* false - otherwise
*/
extern bool addToMoveArray(CH_Game* src,int row, int col,int i, int j,int** arr, int index);
/**
* Returns an initialized two dimensional array
*
* @param size - the expected array size
*
* @return
* array - if there allocation succeded
* NULL - otherwise
*/
extern int** arrayInit(int size);
/**
* Returns true if the game is in tie mode
*
* @param src - the source game
*
* @return
* true - if there is a tie
* false - otherwise
*/
extern bool isTie(CH_Game* src);
/**
* Returns true if the game is in check mode
*
* @param src - the source game
*
* @return
* true - if there is a tie
* false - otherwise
*/
extern bool isCheck(CH_Game* src);
/**
* Returns true if the game is in checkmate mode
*
* @param src - the source game
*
* @return
* true - if there is a tie
* false - otherwise
*/
extern bool isCheckMate(CH_Game* src);
/**
* Updates the pieces' vector according to the game board (the last move)
*
* @param src - the source game
*
*/
extern void piecesUpdate(CH_Game* src,char figure,bool add,char player);
/**
* Returns true if there are no valid possible moves
*
* @param src - the source game
*
* @return
* true - if there is no moves
* false - otherwise
*/
extern bool noPossibleMoves(CH_Game* src);
/**
* Returns maximum number moves
*
* @param ch - the figure
*
* @return
* number of moves
*/
extern int numOfMoves(char ch);
/**
 * Returns which player is the PC
 *
 * @param src - the source game
 *
 * @return
 * CH_PLAYER_1 - if user color is black
 * CH_PLAYER_2 - otherwise
 */
extern char playerPC(CH_Game* src);
/**
 * Returns all possible moves of a specific figure on board
 *
 * @param src - the source game
 * @param row - the row of the figure
 * @param col - the col of the figure
 *
 * @return
 * NULL - if there was an allocation problem
 * arr with (-1,-1) in the first cell if there is no possible moves
 * o.w - arr with the possible moves
 */
extern int** possibleMoves(CH_Game* src, int row, int col);
/**
 * Frees all two dimensional array memory resources
 *
 * @param arr - the array which needed to be freed
 *
 * @return - no return - void function
 */
extern void freeArray(int** arr,int size);
/**
 * Prints all figure's possible moves sorted + *-if threatened, ^-if captured opponent figure
 *
 * @param src - the source game
 * @param row - the row of the figure
 * @param col - the col of the figure
 *
 * @return
 * CH_SUCCESS
 */
extern CH_GAME_MESSAGE getMovesBonus(CH_Game* src, int row, int col,char** board,bool gui);
/**
 * Compares two tuples
 *
 * @param arg1 - the first argument
 * @param arg2 - the second argument
 *
 * @return
 * -1 if a<b
 * o.w 1
 */
extern int comp(const void* arg1, const void* arg2);
/**
 * assigns to the gamecopy the src's fields and params
 * @param src - the game to be copied
 * @param gamecopy - the game to which the data is copied
 *
 * @return
 * false - if src or gamecopy is null
 * true- otherwise
 */
extern bool gameAssign(CH_Game* src,CH_Game* gamecopy);
/**
 * destroys the game in case of a malloc fail + prints a suitable message
 * @param game - the game to be destroyed
 * @param destroy - flag if we need to destroy the game or not
 *
 * @return
 * no return - void function
 */
extern void failGame(CH_Game *game,bool destroy);

#endif