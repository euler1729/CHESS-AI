#ifndef INTRODUCTION_H_
#define INTRODUCTION_H_

#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "Button.h"
#include "Sound.h"

// Button Id
typedef enum{
    INTRO_BUTTON_BACK=11, 
    INTRO_WINDOW_INVALID_ARGUMENT,
    INTRO_WINDOW_EVENT_NONE,
    INTRO_WINDOW_EVENT_QUIT
}INTRO_EVENT;


//WINDOW Size
#define INTRO_WINDOW_WIDTH 900
#define INTRO_WINDOW_HEIGHT 600

#define BUTTON_POS_X 370
#define BUTTON_POS_Y 500
#define BUTTON_H 75
#define BUTTON_W 170



typedef struct intro_window{
    SDL_Window* windowIntro;
    SDL_Renderer* RendererIntro;
    Button* introBackButton;
}Intro_Window;



/**
 * @brief 
 * 
 * @return Intro_Window* 
 */
extern Intro_Window* IntroWindowCreate();

/**
 * @brief 
 * 
 * @param src 
 */
void IntroWindowDestroy(Intro_Window* src);

/**
 * @brief 
 * 
 * @param src 
 */
extern void IntroWindowDraw(Intro_Window* src);

/**
 * @brief 
 * 
 * @param x 
 * @param y 
 * @param src 
 * @return int 
 */
extern int isClickedOnBack(int x, int y, Intro_Window* src);

/**
 * @brief 
 * 
 * @param src 
 */
extern void IntroWindowHide(Intro_Window* src);
/**
 * @brief 
 * 
 * @param src 
 */
extern void IntroWindowShow(Intro_Window* src);
/**
 * @brief 
 * 
 * @param src 
 * @param event 
 * @return INTRO_EVENT 
 */
extern INTRO_EVENT IntroWindowHandleEvent(Intro_Window* src, SDL_Event *event);

#endif