#ifndef INSTRUCTIONWIN_H_
#define INSTRUCTIONWIN_H_


#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "Button.h"
#include "Sound.h"

// Button Id
typedef enum{
    INSTRC_BUTTON_BACK=11, 
    INSTRC_WINDOW_INVALID_ARGUMENT,
    INSTRC_WINDOW_EVENT_NONE,
    INSTRC_WINDOW_EVENT_QUIT
}INSTRC_EVENT;


//WINDOW Size
#define INSTRC_WINDOW_WIDTH 900
#define INSTRC_WINDOW_HEIGHT 600

#define BUTTON_POS_X 370
#define BUTTON_POS_Y 500
#define BUTTON_H 75
#define BUTTON_W 170

// Represents the INSTRUCITON window
typedef struct instruction_window{
    SDL_Window* windowInstrc;
    SDL_Renderer* RendererInstrc;
    Button* InstrcBackButton;
}InstrcWin;



/**
 * @brief The function creates Instruction window, takes no parameter but returns a structure
 * 
 * @return INSTRC_WINDOW* 
 */
extern InstrcWin* InstrcWindowCreate();

/**
 * @brief Destroys the INSTRUCTION winodw
 * 
 * @param src 
 */
void InstrcWindowDestroy(InstrcWin* src);

/**
 * @brief Draws the Instruction window
 * 
 * @param src 
 */
extern void InstrcWindowDraw(InstrcWin* src);

/**
 * @brief determines and return the mouse click position
 * 
 * @param x coordinate of mouse
 * @param y coordinate of mouse
 * @param src 
 * @return int 
 */
extern int isClickedOnBack(int x, int y, InstrcWin* src);

/**
 * @brief Hides the INSTRUCTION window 
 * 
 * @param src 
 */
extern void InstrcWindowHide(InstrcWin* src);
/**
 * @brief Shows the INSTRUCTION Window
 * 
 * @param src 
 */
extern void InstrcWindowShow(InstrcWin* src);
/**
 * @brief Handles events in INSTRUCTION window
 * 
 * @param src 
 * @param event 
 * @return INTRO_EVENT 
 */
extern INSTRC_EVENT InstrcWindowHandleEvent(InstrcWin* src, SDL_Event *event);

#endif