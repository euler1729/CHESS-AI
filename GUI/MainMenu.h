#ifndef MAINMENU_H_
#define MAINMENU_H_

#include <stdio.h>
#include "Button.h"
#include "Panel.h"
#include "Sound.h"

// Buttons defining
#define MAIN_START_BUTTON 0
#define MAIN_LOAD_BUTTON 1
#define MAIN_QUIT_BUTTON 2
#define NUM_OF_MAIN_BUTTONS 3

//WINDOW SIZES
#define MAIN_WINDOW_H 600
#define MAIN_WINDOW_W 900

//COLORS
#define MAIN_R 100
#define MAIN_G 100
#define MAIN_B 100

//BUTTON DIMENSION
#define MAIN_X 75
#define MAIN_Y 25
#define MAIN_H 100
#define MAIN_W 250
#define MAIN_OFFSET 25

//FLAGS
#define ERROR_FLAG_MAIN -1
#define MAIN_ARGS_INIT -1


//represents all possible main events
typedef enum{
    MAIN_EXIT,
    MAIN_START,
    MAIN_INVALID_ARGUMENT,
    MAIN_NONE,
    MAIN_LOAD
}MAIN_EVENT;

//represents main window
typedef struct{
    SDL_Window* mainWindow;//window name
    SDL_Renderer* mainRenderer;//window's renderer
    Button** buttonList; //main window buttons list
    int buttonCounter;//number of buttons
    // Panel* bgPanel;
}MainWin;
/**
 * @brief Function that checks whether there was a click on a button in main screen
 * @param x - x coordinate of click
 * @param y - y coordinate of click
 * @param src - the source main window
 * @return 
 * number that represents the button clicked
 * returns -1 if no button clicked
 */
extern int isClickedOnMain(int x,int y,MainWin* src);
/**
 * @brief Creates a main window, if an error occured, prints a suitable message
 * @return main window, if an error occurs return NULL
 */
extern MainWin* CreateMainWindow();
/**
 * @brief Draws the main window
 * @param src 
 */
extern void DrawMainWindow(MainWin* src);
/**
 * @brief destroys main window
 * 
 * @param src 
 */
extern void DestroyMainWindow(MainWin* src);
extern void MainWindowHide(MainWin* src);
extern void MainWindowShow(MainWin* src);
/**
 * @brief Handles the main window to the main event
 * @param src - th source main window
 * @return 
 * Returns the button number that was clicked
 * The relevent event
 * or MAIN_EVENT_INVELID_ARGUMENT if the is an error
 */
extern MAIN_EVENT HandleMainWindowEvent(MainWin* src, SDL_Event* event);

#endif