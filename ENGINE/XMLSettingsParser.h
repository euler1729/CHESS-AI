#ifndef XMLSETTINGSPARSER_H_
#define XMLSETTINGSPARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define SP_MAX_LINE_LENGTH 1024
#define PATH_COMMAND_FLAG 2
#define INT_COMMANDS_FLAG 1

//retpresents a command
typedef enum
{
    CH_INVALID_LINE,
    CH_GAME_MODE,
    CH_DIFFICULTY,
    CH_USER_COLOR,
    CH_LOAD,
    CH_DEFAULT,
    CH_QUIT,
    CH_START,
    CH_PRINT_SETTING
} SET_COMMAND_NAME;

//used to enapsulte a parsed line
typedef struct command_t
{
    SET_COMMAND_NAME cmd;
    bool validArg; //set to true if the line contains a valid argument
    int arg;       // argument for a command with a number (e.g. difficulty)
    char path[SP_MAX_LINE_LENGTH];
} SetCommand;
/**
 * @brief Checks which command the user wrote.
 * The function gets a string that represents the command,
 * if the command is unknown returns INVALID_LINE
 * @return 
 * enum that represents th command - if the command is valid.
 */
extern SET_COMMAND_NAME parserIsSetCommand(const char *str);
/**
 * @brief Checks if a specified string represents a valid integer.
 * @return 
 * true if the string represents a valid integer, 
 */
extern bool parserIsInt(const char *str);
/**
 * @brief 
 * Parses a specified line.
 * If the line is a command which has an integer argument then the argument is parsed
 * and is saved in the field arg and the field validArg is set to true.
 * If teh line a command which has an argument that represents a path then the argument is parsed
 * and is saved in the field path and the field validArg is set to true.
 * If the line is a command which has an argument that represents a move then the argumetn is parsed 
 * and is saved in the field move and the field validArg set to true.
 * In any other case then 'validArg' is set to false and the values 'arg', 'path' and 'move' are undefined
 * @return 
 * A parsed line such that
 *  cmd - contains the command type, if ;the line is invalid then this field is set to INVALID_LINE.
 * validArg - is set to true if the command has a relevebt extra information: path, number or move';
 * arg - the integer argument in case validAarg is set to true
 */
extern SetCommand parserSetParseLine(char *str);

#endif