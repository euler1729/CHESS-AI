#ifndef CONSOLEMODE_H_
#define CONSOLEMODE_H_

#include <stdlib.h>
#include "Search.h"
#include "XMLREADWRITE.h"
#include "ASSERT.h"


#define EXPERT_LEVEL 7
#define TWO_PLAYER_MODE 2

#define toChar(a) (a+65)
/**
 * @brief Initialize the Game
 * 
 */
extern void startGame();
/**
 * @brief invokes the setting state - if the user enters a valid command
 * game_mode, usee_color, load, difficulty, quit, default or print etc.
 * until the start command is entered
 */
extern void settingState(CH_Game* game);
/**
 * @brief 
 * invokes the game state - the user enters a valid game command:
 * move, get moves, save, undo reset
 * until the quit commnad is entered.
 * @param game - the src game
 * @param command - the game command the user entered
 * @return
 * false is case of an error
 * otherwise - true
 */
extern bool gameState(CH_Game* game, GameCommand command);
/**
 * @brief 
 * returns the set command that the user entered after being parsed
 * @return - set command as listed in the enum
 */
extern SetCommand getNewSetCommand();
/**
 * @brief 
 * returns the game command that the user entered afer being parsed
 * @param game - the src game
 * @param to_print - flag that determines whether to print the board or not
 * @return - setcommand as listed in the enum
 */
extern GameCommand getNewGameCommand(CH_Game* game, bool to_print);
/**
 * @brief 
 * changes the game mode according to the user's wishes, prints a suitable message
 * @param game - the src game
 * @param arg - the game mode that the user wants
 * 
 */
extern void gameMode(CH_Game* game, int arg);
/**
 * @brief 
 * Change the game difficulty according to the user's wishes, prints a suitable message.
 * @param game - the src game
 * @param arg - the game difficulty that the user wants
 */
extern void gameDifficulty(CH_Game* game, int arg);
/**
 * @brief 
 * Loads a game from the file that the user specified and prints a message if there was an error
 * @param game
 * @param path - the file path that the user specifies
 */
extern void gameLoad(CH_Game* game, char* path);
/**
 * @brief sets the game default params
 * @param game
 */
extern void gameDefault(CH_Game* game);
/**
 * @brief prints the game setting according to the mode
 * @param game
 */
extern void gamePrintSetting(CH_Game* game);
/**
 * @brief QUits the game and frees all resources
 * @param game 
 * @param check - boolen if true than removes all
 */
extern void gameQuit(CH_Game* game, bool check);
/**
 * @brief sets a game move - moves the players' piece according to the gives move,
 * checks whether the move is valid and prints a message accordingly
 * @param game
 * @param move - an array that represent the src cell and destination cell
 * @return 
 * false - if the move cannot be done
 * else true
 */
extern bool gameMove(CH_Game* game, int* move);
/**
 * @brief Prints all possible game moves of the given piece
 * checks whether the invoke of the command is valid and pritns accordingly
 * @param game
 * @param move - an array that represents the pieces cell
 * @return false - if the command is illegal
 */
extern bool gameGetMoves(CH_Game* game, int* move);
/**
 * @brief saves the current game to the given path
 * tries to do the command is valid and prints accordingly
 * @param game
 * @param path - represents the destination file
 * @return  
 * false if commmand failed
 */
extern bool gameSave(CH_Game* game, char* path);
/**
 * @brief undo last 2 moves in the game
 * @param game
 * @return - false is command failed
 */
extern bool gameUndoMove(CH_Game* game);
/**
 * @brief resets the game to the setting mode
 * @param game
 */
extern void gameReset(CH_Game* game);
/**
 * @brief prints a message if there was an allocation error
 * @param  game
 * @param bool - flag that says whether to destroy the game or not
 */
extern void fail(CH_Game* game, bool destroy);
/**
 * @brief Functoin that calls the miniMAx and computes the best move for the computer
 * the function makes that move and prints the information
 * @param game
 */
extern void computerMove(CH_Game* game);
/**
 * @brief mamagement of one player mode:
 * gets the command, check whether there was a winner and executes the command given by the user
 * makes the computer move
 * @param game
 */
extern void onePlayerMode(CH_Game* game);
/**
 * @brief management of two player mode:
 * get the commands, checks whether there was a winner and executes the command given by the user
 * @param game
 * 
 */
extern void twoPlayerMode(CH_Game* game);
/**
 * @brief the function returns the player's color according to the char that it represents 
 * @param ch - the player char
 * @return black if ch =='F else white if ch=='S'
 * 
 */
extern char* color(char ch);
/**
 * @brief  The function returns the figure name acoording to the char that it represents 
 * @param ch - the player
 * @return 
 * pawn - P/p
 * rook - R/r
 * knight - N/n
 * bishop  B/b
 * Queen - Q/q
 * King - K/k
 * NULL - otherwise 
 */
extern char* figure(char ch);
/**
 * @brief checks whether the game status changed to tie/check/cjeckmate and prints a message
 * @param game
 * 
 */
extern void statusCheck(CH_Game* game);
/**
 * changes the game difficulty according to the user's wishes, prints a suitable message
 *
 * @param game - the src game
 * @param arg - the game difficulty that the user wants - only 0 or 1
 *
 * no return - a void function
 */
extern void gameColor(CH_Game* game, int arg);

#endif