#ifndef GUIMODE_H_
#define GUIMODE_H_


#include <stdio.h>
#include "GUIManager.h"
#include "Sound.h"

/**
 * @brief Starts the GUI MODE
 * 
 * @return int 
 */
extern int startGuiMode(); 
/**
 * @brief retruns the current state of the game, is there any check, or is there any winner or a normal state
 * 
 * @param manager 
 * @return int 
 */
extern int currentState( GuiManager* manager); 


#endif