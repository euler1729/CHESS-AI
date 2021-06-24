#ifndef XMLGAMEPARSER_H_
#define XMLGAMEPARSER_H_


#include <stdio.h>
#include <stdalign.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>


#define SP_MAX_LINE_LENGTH 1024 
#define PATH_COMMANDS_FLAG_GAME 3
#define MOVES_COMMANDS_FLAG 2
#define ASCII_OFFSET 64
#define INVALID_POSITION_FLAG 9

#ifndef CELL_SIZE
    #define CELL_SIZE 2
#endif //CELL_SIZE

// a type used to represent a command
typedef enum{
    CH_GINVALID_LINE,
    CH_MOVE,
    CH_SAVE,
    CH_UNDO,
    CH_RESET,
    CH_GET_MOVES,
    CH_GQUIT
}GAME_COMMAND_NAME;

//that is used to encapsulate a parsed line
typedef struct command_gt{
    
    GAME_COMMAND_NAME cmd;
    bool validArg; // set to true if the line contains a valid argument
    char path[SP_MAX_LINE_LENGTH];//argument for a command with a folder path
    int move[CELL_SIZE*CELL_SIZE];//move is represented by 2 cells = 4 CoOrdinates

}GameCommand;

/**
 * @brief Checks if a specified string represents a valid tuple,
 * @return 
 * returns true if string represents valid tuple else false
 */
extern bool parserIsTuple(const char* str, GameCommand *curr, int cell);
/**
 * @brief 
 * Parses a specified line.
 * If the line is a command which has an argument that represents a path then the argument is parsed
 * and is saved in the field path and the field validArg is set to true.
 * In any other case then 'validArg' is set to false and the values 'path' and 'move' are undefined.
 * @return 
 * A parsed line such that :
 * cmd - contains the command type, if the line is invalid then this field is set to INVALID_LINE
 * validArg - is set to true if the command has a relevant extra informaiton: path, number of moves.
 * path - the path argument in case validArg is set to true
 * move - the move argument in case validArg is set to true
 */
extern GameCommand parserGameParseLine(char* str);
/**
* checks which command the user wrote. The function gets a string that represents the command,
*  if the command is unknown  returns INVALID_LINE
* @return
* Enum that represents the command - if the command is valid
* CH_GINVALID_LINE - otherwise
*/

extern GAME_COMMAND_NAME parserIsGameCommand(const char* str);


#endif