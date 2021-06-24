#ifndef XMLREADWRITE_H_
#define XMLREADWRITE_H_


#include "ChessGame.h"
#include "XMLSettingsParser.h"
#include "XMLGameParser.h"
#include <unistd.h>
#include <string.h>


typedef enum xml_messages
{
    XML_SUCCESS,
    XML_FAILED,
    XML_ALLOC
} XML_MESSAGE;

/**
 * @brief 
 * gets a game and writes to file the game params according to the Conventions
 * @param game - the game that should be saved
 * @param f - the  destination file
 * @return 
 * XML_FAILED - in case of an error
 * otherwise - XML_SUCCESS 
 */
extern XML_MESSAGE gameToFile(CH_Game *src, FILE *xml);
/**
 * @brief 
 * return a string tht represent a gameboard row
 * @param src -  the game that should be saved
 * @param row - the line that needs to be written to file
 * @return 
 * 6 characters string that represent the row
 */
extern char *boadRowToLine(CH_Game *src, int row);
/**
 * @brief
 * gets a file and parser each line that represents game's params
 * 
 * @param f - the src file
 * @return - the game that the file represents
 */
extern CH_Game *fileToGame(FILE *f);

#endif