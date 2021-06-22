#ifndef LOADWINDOW_H_
#define LOADWINDOW_H_

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "Button.h"
#include "../ENGINE/ConsoleMode.h"
#include "Sound.h"


//enum that represents all load events possible
typedef enum
{
	lOAD_EVENT_BACK,
	lOAD_EVENT_lOAD,
	lOAD_EVENT_SLOT1,
	lOAD_EVENT_SLOT2,
	lOAD_EVENT_SLOT3,
	lOAD_EVENT_SLOT4,
	lOAD_EVENT_SLOT5,
	lOAD_EVENT_INVALID_ARGUMENT,
	lOAD_EVENT_NONE,
	LOAD_EVENT_QUIT
} LOAD_EVENT;



//BUTTONS
#define SLOT1_BUTTON 0
#define SLOT2_BUTTON 1
#define SLOT3_BUTTON 2
#define SLOT4_BUTTON 3
#define SLOT5_BUTTON 4

#define LOAD_BACK_BUTTON 5
#define LOAD_LOAD_BUTTON 6

//positional coordinate for xml data slot button
#define SLOTS_X_VAL 350 
#define SLOTS_Y_VAL 50
#define SLOTS_Y_OFFSET 15
#define SLOTS_H 50
#define SLOTS_W 200

#define OTHER_H 75
#define OTHER_W 170
#define OTHER_Y 450
#define BACK_X 250
#define LOAD_X 500

//COLORS DEFINING
#define LOAD_R 100
#define LOAD_G 100
#define LOAD_B 100

//WINDOW SIZES
#define LOAD_WINDOW_H 600
#define LOAD_WINDOW_W 900

//MASC DEFINING
#define NUM_OF_LOAD_BUTTONS 7
#define SLOT_BUTTONS_START_INDEX 2

//FLAGS
#define ERROR_FLAG_LOAD -1
#define LOAD_ARGS_INIT -1


//Represents load window
typedef struct
{
	SDL_Window *windowLoad;		//winodow name
	SDL_Renderer *rendererLoad; //window's renderer
	Button **buttonList;		//load window button
	int buttonCounter;
} LoadWin;

/**
 *  function that checks whether there was a click on a button in load screen
 *	@param x - the x coordinate of the click
 *	@param y - the y ccoordinate of the click
 *	@param src - the source load window
 *  @return
 *  number that represents the button clicked
 *  -1 - if no button was clicked
 */
extern int isClickedOnLoad(int x, int y, LoadWin *src);
/**
 *  Creates a load window, if an error occured, prints a suitable message.
 *
 *	@param num_of_saved_games - number of games saved until now.
 *
 *  @return
 *  NULL, if an allocation error occurred
 *  load window - otherwise
 */
extern LoadWin *LoadWindowCreate(int num_of_saved_games);
/**
 *  function that draws the load window, prints a suitable message if src == NULL
 *  @param src - the source load window
 */
extern void LoadWindowDraw(LoadWin *);
/**
 * Frees all memory resources associated with the source window. If the
 * source window is NULL, then the function does nothing.
 * @param src - the source window
 */
extern void LoadWindowDestroy(LoadWin *src);
/**
 *  Handles the load window to the load event
 *	@param event - the event that happened in the load window
 *	@param game - the source game
 *	@param src - the source load window
 *  @return
 *  SP_lOAD_EVENT_INVALID_ARGUMENT - if one of the arguments is invalid
 *  otherwise -the relevant event
 */
extern LOAD_EVENT LoadWindowHandleEvent(LoadWin *src, SDL_Event *event);
/**
 *  initializes the buttons on load window
 *	@param num_of_saved_games - how many games are saved in folder right now
 *	@param win - the source load window
 *  @return
 *  true - if succeeded
 *  false -if an error occurred
 */
extern bool loadButtonInit(LoadWin *win, int num_of_saved_games);
/**
 *  Updates the buttons according to a click on a valid game slot
 *	@param src - the source load window
 *	@param event - the number of the game slot which was clicked
 *
 */
extern void activateAfterClick(LoadWin *src, int event);

/**
 *  Hides the relevant window
 *	@param src - the source load window
 *
 */
extern void LoadWindowHide(LoadWin *src);

/**
 *  Shows the relevant window
 *	@param src - the source load window
 *
 */
extern void LoadWindowShow(LoadWin *src);

#endif