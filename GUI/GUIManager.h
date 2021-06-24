#ifndef GUIMANAGER_H_
#define GUIMANAGER_H_


#include <stdio.h>
#include <stdlib.h>
#include "MainMenu.h"
#include "GameWindow.h"
#include "LoadWindow.h"
#include "SettingWindow.h"
#include "InstructionWin.h"
#include "Sound.h"


#define MAX_OF_SAVED_FILES 5
#define NUM_OF_IMAGES 15

#define FILE_SLOT_1 0
#define FILE_SLOT_2 1
#define FILE_SLOT_3 2
#define FILE_SLOT_4 3
#define FILE_SLOT_5 4

//enum that represents all existing windows
typedef enum
{
	MAIN_WINDOW,
	GAME_WINDOW,
	LOAD_WINDOW,
	SETTINGS_WINDOW,
	INSTRC_WINDOW
} WINDOWS_LIST;

// represents events like quit window
typedef enum
{
	MANAGER_QUIT,
	MANAGER_NONE,
} MANAGER_EVENT;

//represents the gui manager
typedef struct
{
	GameWindow *GameWindow;	  //game window
	MainWin *mainWin;	      //main window
	LoadWin *loadWin;	      //load window
	SettingsWin *settingsWin; //settings window
	InstrcWin* InstrWin;	  //Instruction Window
	WINDOWS_LIST activeWin;	  //which window is active at the moment
	WINDOWS_LIST prevWin;	  //previous window
	CH_Game *game;			  //chess game
	const char *board_images[NUM_OF_IMAGES];
	const char *saved_games[MAX_OF_SAVED_FILES];

} GuiManager;

/**
 *  Creates a gui manager
 *  @return
 *  NULL, if an allocation error occurred
 *  gui manager - otherwise
 */
extern GuiManager *ManagerCreate();
/**
 * Frees all memory resources associated with the source gui manager. If the
 * source gui manager is NULL, then the function does nothing.
 * @param src - the source gui manager
 */
extern void ManagerDestroy(GuiManager *src);
/**
 * Draws the active window
 * @param src - the source gui manager
 */
extern void ManagerDraw(GuiManager *src);
/**
 *  Handles the different events from the different windows.
 *  handles manager due to manager events (show/hide window)
 *  prints an error message if an allocation error occured
 *
 * @param src - the source gui manager
 * @param event - sdl event
 *
 *  @return
 *  SP_MANAGER_QUTT, if there was a click on X or there was an allocation error
 *  SP_MANAGER_NONE, if src==NULL or event that wasn't mentioned above
 */
extern MANAGER_EVENT ManagerHandleEvent(GuiManager *src, SDL_Event *event);
/**
 *  function that handles all the events occurrs on game window
 * @param src - the source gui manager
 * @param event - the event occurred
 * @return
 * SP_MANAGER_EVENET
 */
MANAGER_EVENT handleManagerDueToGameEvent(GuiManager *src, GAME_EVENT event);

/**
 *  function that makes the needed changes following load event click
 * @param src - the source gui manager
 * @return
 * MANAGER_EVENT
 */
extern MANAGER_EVENT loadEvent(GuiManager *src);
/**
 *  Handles the different events that load has.
 *  handles manager due to load events (show/hide window)
 *  prints an error message if an allocation error occured
 * @param src - the source gui manager
 * @param event - game event
 *  @return
 *  MANAGER_QUTT, if there was a click on X or there was an allocation error
 *  MANAGER_NONE, if src==NULL or event that wasn't mentioned above
 */
extern MANAGER_EVENT handleManagerDueToLoadEvent(GuiManager *src, LOAD_EVENT event);
/**
 *  Handles the different events that main has.
 *  handles manager due to main events (show/hide window)
 *  prints an error message if an allocation error occured
 *
 * @param src - the source gui manager
 * @param event - main event
 *
 *  @return
 *  MANAGER_QUTT, if there was a click on X or there was an allocation error
 *  MANAGER_NONE, if src==NULL or event that wasn't mentioned above
 */
extern MANAGER_EVENT handleManagerDueToMainEvent(GuiManager *src, MAIN_EVENT event);
/**
 *  Handles the different events that settings has.
 *  handles manager due to settings events (show/hide window)
 *  prints an error message if an allocation error occured
 *
 * @param src - the source gui manager
 * @param event - settings event
 *
 *  @return
 *  MANAGER_QUTT, if there was a click on X or there was an allocation error
 *  MANAGER_NONE, if src==NULL or event that wasn't mentioned above
 */
extern MANAGER_EVENT handleManagerDueToSetEvent(GuiManager *src, SETTINGS_EVENT event);

/**
 *  Draws next settings sub screen according to the flow
 *  Prints suitable message if src == NULL
 *
 * @param src - the source gui manager
 *
 */
extern void drawSetNextScreen(GuiManager *src);

/**
 *  Draws previous settings sub screen according to the flow
 *  Prints suitable message if src == NULL
 * @param src - the source gui manager
 */
extern void drawSetPrevScreen(GuiManager *src);

/**
 *  init the game board images gui manager field
 *
 * @param src - the source gui manager
 *
 */
extern void updateImages(GuiManager *src);

/**
 * computes number of saved games
 *
 * @param src - the source gui manager
 *
 * @return
 * number of games that the user saved
 */
extern int num_of_saved_files(GuiManager *src);

/**
 * saves the gui game board to xml file to be used in the future
 *
 * @param src - the source gui manager
 *
 * @return
 * XML_SUCCESS - on success of save
 * XML_FAILED - otherwise
 */
extern XML_MESSAGE saveGameGui(GuiManager *src);

/**
 * saves the gui game board to xml file to be used in the future
 *
 * @param src - the source gui manager
 *
 * @return
 * XML_SUCCESS - on success of save
 * XML_FAILED - otherwise
 */
extern void saveUpdate(GuiManager *src);

/**
 * initializes all save games paths
 * @param src - the source gui manager
 */
extern void initSaves(GuiManager *src);

/**
 *  function that undoes the previous two moves on the game board
 * @param src - the source gui manager
 */
extern void undoGameGui(GuiManager *src);

/**
 *  function that restarts the game board
 * @param src - the source gui manager
 * @return
 * MANAGER_EVENET
 *
 */
extern MANAGER_EVENT gameRestart(GuiManager *src);
/**
 * @brief Handles Eveent occurs in Instruction window
 * 
 * @param src 
 * @param event 
 * @return MANAGER_EVENT 
 */
extern MANAGER_EVENT HandleEventDueToInstrWindow(GuiManager* src, INSTRC_EVENT event);

#endif