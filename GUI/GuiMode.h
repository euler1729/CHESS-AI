#ifndef GUIMODE_H_
#define GUIMODE_H_

#include <stdio.h>
#include "GUIManager.h"
#include "Sound.h"

extern int startGuiMode(); //Starts the GUI MODE
extern int currentState( GuiManager* manager); // retruns the current state of the game, is there any check, or is there any winner or a normal state


#endif