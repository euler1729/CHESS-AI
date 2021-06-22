#ifndef SETTINGWINDOW_H_
#define SETTINGWINDOW_H_

#include <stdio.h>
#include "Button.h"
#include "../ENGINE/ConsoleMode.h"
#include "Sound.h"




/**enums that represents all settings events possible **/

//Represents all possible setting events
typedef enum {
	//general
	SETTINGS_EVENT_BACK,
	SETTINGS_EVENT_START,
	SETTINGS_EVENT_NEXT,
	//game mode
	SETTINGS_EVENT_ONE_PLAYER,
	SETTINGS_EVENT_TWO_PLAYER,
	//difficulty
	SETTINGS_EVENT_NOOB,
	SETTINGS_EVENT_EASY,
	SETTINGS_EVENT_MODERATE,
	SETTINGS_EVENT_HARD,
	//color
	SETTINGS_EVENT_WHITE,
	SETTINGS_EVENT_BLACK,
	//errors
	SETTINGS_EVENT_INVALID_ARGUMENT,
	SETTINGS_EVENT_NONE,
	SETTINGS_EVENT_QUIT
} SETTINGS_EVENT;

//retpresents the sub-setting windows 
typedef enum{
    //General
    GameModeScreen,
    DifficultyScreen,
    ColorScreen
}SUB_WINDOW;



// BUTTONS DEFINING
#define NOOB_BUTTON 0
#define EASY_BUTTON 1
#define MODERATE_BUTTON 2
#define HARD_BUTTON 3
#define WHITE_BUTTON 4
#define BLACK_BUTTON 5
#define TWO_PLAYER 6
#define ONE_PLAYER 7
#define SET_BACK_BUTTON 8
#define SET_START_BUTTON 9
#define SET_NEXT_BUTTON 10

#define NUM_OF_SET_BUTTONS 11
#define NUM_OF_DIFFICULTIES 4

#define DIF_X 375
#define DIF_H 50
#define DIF_W 150
#define DIF_GAP 20
#define DIF_START_Y 100

#define PLAYERS_W 220
#define PLAYERS_H 100
#define PLAYERS_Y 100
#define PLAYERS_1X_BUTTON 220
#define PLAYER_2X_BUTTON 480

#define WHITE_X_BUTTON 235
#define BLACK_X_BUTTON 520
#define COLOR_BUTTON_Y 160
#define COLOR_BUTTON_H 150


#define OTHERS_BUT_WIT 180
#define OTHERS_BUT_H 75
#define OTHERS_BUT_Y 500
#define OTHERS_BUT_X 600



//WINDOW SIZES
#define SET_WINDOW_H 600
#define SET_WINDOW_W 900

//COLORS DEFINING
#define SET_R 100
#define SET_G 100
#define SET_B 100
#define COLOR_R 100
#define COLOR_G 100
#define COLOR_B 100

//MASC
#define SET_DRAG_INIT -1


// Represents Setting window
typedef struct setting_window{
    SDL_Window* window;	   //window name
    SDL_Renderer* renderer;// window's renderer
    Button** buttonList;   //setting window button list
    int buttonCounter;     //Keeps the count of all buttons in the setting window
    SUB_WINDOW curr_screen;//current sub-window
}SettingsWin;

/**
 *  Creates a settings window, if an error occured, prints a suitable message.
 *  @return
 *  NULL, if an allocation error occurred
 *  settings window - otherwise
 */
extern SettingsWin* SettingsWindowCreate();
/**
 *  function that checks whether there was a click on a button in settings screen
 *	@param x - the x coordinate of the click
 *	@param y - the y ccoordinate of the click
 *	@param src - the source settings window
 *  @return
 *  number that represents the button clicked
 *  -1 - if no button was clicked
 */
extern int isClickedOnSetting(int x, int y,SettingsWin* src);
/**
 *  function that draws the settings window, prints a suitable message if src == NULL
 *  @param src - the source settings window
 *
 */
extern void SettingsWindowDraw(SettingsWin* src);
/**
 * Frees all memory resources associated with the source window. If the
 * source window is NULL, then the function does nothing.
 * @param src - the source window
 */
extern void SettingsWindowDestroy(SettingsWin* src);

/**
 *  Handles the load window to the settings event
 *	@param event - the event that happened in the settings window
 *	@param game - the source game
 *	@param src - the source settings window
 *
 *  @return
 *  SP_SETTINGS_EVENT_INVALID_ARGUMENT - if one of the arguments is invalid
 *  otherwise -the relevant event
 */
extern SETTINGS_EVENT SettingsWindowHandleEvent(SettingsWin* src, SDL_Event* event,CH_Game* game);
/**
 *  Hides the relevant window
 *	@param src - the source settings window
 *
 */
extern void SettingsWindowHide(SettingsWin* src);

/**
 *  Shows the relevant window
 *	@param src - the source settings window
 *
 */
extern void SettingsWindowShow(SettingsWin* src);
/**
 *  Changes the screen to the relevant sub screen - game mode
 *  Shows the relevant buttons
 *
 *	@ param src - the source settings window
 *	@ param game - the chess game
 *	@ param one player - flag that shows if the buttons clicked is one player button
 *
 */
extern void gameModeChanges(SettingsWin* src, CH_Game* game,bool one_player);
/**
 *  Changes the screen to the relevant sub screen - difficulty
 *  Shows the relevant buttons
 *
 *	@ param src - the source settings window
 *	@ param game - the chess game
 *	@ param diff - represents the difficulty choosen
 *
 */
extern void gameDiffChanges(SettingsWin* src, CH_Game* game,int diff);
/**
 *  Changes the screen to the relevant sub screen - color
 *  Shows the relevant buttons
 *
 *	@ param src - the source settings window
 *	@ param game - the chess game
 *	@ param color - represents the color choosen
 *
 */
extern void gameColorChanges(SettingsWin* src, CH_Game* game,int color);

/**
 *  Helper function that switches to the relevant screen - difficulty
 *	@param src - the source settings window
 *
 */
extern void SettingsChangeToDifficulty(SettingsWin* src);

/**
 *  Helper function that switches to the relevant screen - game mode
 *	@param src - the source settings window
 *
 */
extern void SettingsChangeToGameMode(SettingsWin* src);

/**
 *  Helper function that switches to the relevant screen - color
 *	@ param src - the source settings window
 */
extern void SettingsChangeToColor(SettingsWin* src);

/**
 *  initializes settings buttons
 *	@param res - the source settings window
 *
 *  @return
 *  true - if succeeded
 *  false -if an error occurred
 */
extern bool setButtonsInit(SettingsWin* win);



#endif