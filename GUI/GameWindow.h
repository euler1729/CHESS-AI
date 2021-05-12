#ifndef GameWindow_H_
#define GameWindow_H_

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include "../ConsoleMode.h"
#include "Button.h"
#include "Panel.h"
#include <SDL2/SDL_mixer.h>
#include "Sound.h"

//Represernts all events in game window
typedef enum
{
	GAME_EVENT_PLAYER_1_WON,
	GAME_EVENT_PLAYER_2_WON,
	GAME_EVENT_TIE,
	GAME_EVENT_CHECK,
	GAME_EVENT_QUIT,
	GAME_EVENT_RESTART,
	GAME_EVENT_LOAD,
	GAME_EVENT_MAIN_MENU,
	GAME_EVENT_INVALID_ARGUMENT,
	GAME_EVENT_SAVE_EXIT,
	GAME_EVENT_SAVE,
	GAME_EVENT_SAVE_MAIN_MENU,
	GAME_EVENT_UNDO,
	GAME_EVENT_GET_MOVES,
	GAME_EVENT_NONE
} GAME_EVENT;

#define borderX(a) ((abs(a - 800)) <= (abs(a - 200)) ? (770) : (230))
#define borderY(a) ((abs(a - 600)) <= (a) ? (570) : (30))
#define xValue(a, b, c) ((a - b) / c)
#define yValue(a, b, c) ((a - 1 - b) / (c - 1))
#define dist(a, b) (sqrt((a * a) + (b * b)))

// BUTTONS DEFINING
#define RESTART_GAME_BUTTON 64
#define SAVE_GAME_BUTTON 65
#define LOAD_GAME_BUTTON 66
#define UNDO_GAME_BUTTON 67
#define MAIN_MENU_GAME_BUTTON 68
#define EXIT_GAME_BUTTON 69

// LOCATION ON WINDOW DEFINING
#define BOARD_START_X 227
#define BOARD_END_X 773
#define BOARD_START_Y 27
#define BOARD_END_Y 573
//WINDOW SIZES DEFINING
#define BOARD_PANEL_START_X 200
#define BOARD_PANEL_START_Y 1
#define BOARD_HEIGHT 600
#define BOARD_WIDTH 600
#define SET_WINDOW_START_X 0
#define SET_WINDOW_START_Y 0
#define SET_HEIGHT 600
#define SET_WIDTH 600
#define WINDOW_HIGHT 600
#define WINDOW_WIDTH 800

//BOARD SIZES DEFINING
#define BOARD_CELL_DIM 70
#define CELL_OFFSET 34
#define BOARD_MIN_X 230
#define BOARD_MAX_X 770
#define BOARD_MIN_Y 30
#define BOARD_MAX_Y 570

// ERRORS FLAG DEFINING
#define ERROR_FLAG -1

//DELAYS DEFINING
#define PC_MOVE_DELAY 1500

//COLORS DEFINING
#define BLACK 0
#define WHITE 255
#define BOARD_R 100
#define BOARD_G 100
#define BOARD_B 100

//INIT VALUES DEFINING
#define GAME_ARGS_INIT -1

//MESSAGE BOX STATUS DEFINING
#define CHECK_FLAG 0
#define RESTART_BUTTONID 1
#define QUIT_BUTTONID 2
#define YES_BUTTONID 1
#define NO_BUTTONID 2
#define CANCEL_BUTTONID 3

//IMAGES NUM INDEX DEFINING
typedef enum
{
	WHITE_PAWN_IMG,
	WHITE_BISHOP_IMG,
	WHITE_ROOK_IMG,
	WHITE_KNIGHT_IMG,
	WHITE_QUEEN_IMG,
	WHITE_KING_IMG,
	BLACK_PAWN_IMG,
	BLACK_BISHOP_IMG,
	BLACK_ROOK_IMG,
	BLACK_KNIGHT_IMG,
	BLACK_QUEEN_IMG,
	BLACK_KING_IMG,
	RED_CELL_IMG,
	YELLOW_CELL_IMG,
	GREEN_CELL_IMG
} PieceImagesId;

//GET MOVES IMG CHARS
#define DEFAULT_CELL '3'
#define RED_CELL '1'
#define YELLOW_CELL '2'
#define GREEN_CELL '0'

// MOVE MACROS
#define QUIT 0
#define RESTART 1
#define NONE 2
#define NO_EVENT 555
#define DRAG_TIME 5

//OTHERS
#define HISTORY_MIN_LIMIT 2

//Represents the game window
typedef struct
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	Panel *settingPanel; //
	Panel *boardPanel;	 //
	Cell *moving_cell;	 //cell that is moving
	CH_Game *game;		 // whole board information stores
	bool to_drag;		 //flag that represents whether to drag or not

	//initial and target square
	int source_x;
	int source_y;
	int target_x;
	int target_y;
	bool save_the_game; //if there is need to offer to save the game
	bool check_printed;
} GameWindow;

/**
 * @brief Creates a game window, if an error occured, prints a suitable message
 * @param game - the source game
 * @param board_game - all board_images
 * @param board_images - the board images that can be used during the update
 * @return 
 * NULL if allocation error
 * else game window
 */
extern GameWindow *GameWindowdowCreate(CH_Game *game, const char **board_images);

/**
 * @brief Function that draws the game window
 * @param src
 */
extern void GameWindowdowDraw(GameWindow *src);
/**
 * @brief Frees all memory resources associated with the source window
 * @param src
 */
void GameWindowdowDestroy(GameWindow *src);
/**
 * @brief Handles the game window according to the game event
 * @param src
 * @param event - event that happended in the game window
 * @param board_images - the board images that can be shown according to each event
 * @return relevent event
 */
extern GAME_EVENT GameWindowdowHandleEvent(GameWindow *src, SDL_Event *event, const char **board_images);

/**
 * @brief Hides the relevant window
 * @param src - the source game window
 */
extern void GameWindowdowHide(GameWindow *src);
/**
 * @brief Shows the relevant window
 * @param src - the source game window
 * 
 */
extern void GameWindowdowShow(GameWindow *src);
/**
 * @brief Checks whether there was a click on a button/celll in window screen
 * @param x - x coordinate of the click
 * @param y - y coordinate of the click
 * @param sec - the source game window
 * @return  number that button/cell was clicked
 * 
 */
extern int isClickedOnGame(int x, int y, GameWindow *src);

/**
 * @brief Updates the board textures that are shown according to the current state
 * @param panel - the relevant to be updated 
 * @param game - the relevant chess game
 * @param board_image - the board images that can be used during the update
 * 
 */
extern int boardUpdate(Panel *panel, CH_Game *game, const char **board_images);
/**
 * @brief Gets a char that represents a game board cell and return the relevant image to be presented on board
 * @param ch - the char that represents a cell in the game board
 * @param board_images - the board images that can used
 * 
 */
const char *cellToImage(char ch, const char **board_images);
/**
 * @brief Updated the board after the player invokes the get moves command (mouse right  click)
 * @param src - the source chess game
 * @param row - the row of the piece that  we want to see it's available moves
 * @param col - the col of the piece that we want to see it's available moves
 * @param board_images - the board images that can be used
 * @return true/false
 * 
 */
extern bool getMovesGui(GameWindow *src, int row, int col, const char **board_images);
/**
 * @brief Initialized the drag argument to -1 (as deafault)
 * @param src
 */
extern void initDragArgs(GameWindow *src);
/**
 * @brief Gets a pixel and returns the index of the relevant cell
 * @param x - the x coordinate of the pixel 
 * @param y - the y coordinate of the pixel
 * @return 
 * the cell index
 */
extern int pixelToIndex(int x, int y);
/**
 * @brief makes computer move
 * @param src
 * @return 0/1
 */
extern int PCMove(GameWindow *src, const char **board_images);
/**
 * @brief prints game status message
 * @return buttonid/-1
 * 
 */
extern int statusMessage(GameWindow *src);
/**
 * @brief prints exit message(would you like to save the game)
 * @param src
 * @param flag - true if the payer wants to exit
 * @return 
 *  SP_GAME_EVENT_QUIT - in case the player wants to quit
 *	SP_GAME_EVENT_MAIN_MENU - in case the player wants to go to the main menu
 *	SP_GAME_EVENT_SAVE - in case the player wants to save and quit
 *	SP_GAME_EVENT_SAVE_MAIN_MENU - in case the player wants to save and go to main menu
 *	SP_GAME_EVENT_NONE - otherwise
 */
extern GAME_EVENT exitMessage(GameWindow *src, bool flag);
/**
 * @brief Update undo button if needed
 * @param panel - the panel
 * @param game - the sourc game
 * 
 */
extern void undoUpdate(Panel *panel, CH_Game *game);
/**
 * @brief makes comouter move
 * @param src
 * @param cell_src - the source cell of the event
 * @param win - the place of the event
 * @param board_images - the images of game board
 * @return 
 * 0 - if need to quit
 * 1 - if need to restart
 * 2 - if none
 */
extern int Moving(GameWindow *src, int cell_src, int win, const char **board_images);
/**
 * @brief updates dragging params
 * @param src
 */
extern void Drag(GameWindow *src);
/**
 * @brief Makes the necessary changes following button up event
 * @param src
 * @param event
 * @param win
 * @param board_images
 * @return GAME_EVENT
 */
extern GAME_EVENT buttonUp(GameWindow *src, SDL_Event *event, int win, const char **board_images);
/**
 * @brief makes the necessary changes following button down event
 * @param src 
 * @param event 
 * @param win
 * @param board_images
 * @return 0 if quit else NO_EVENT
 */
extern int buttonDown(GameWindow *src, SDL_Event *event, int win, const char **board_images);

#endif