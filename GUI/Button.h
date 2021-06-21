#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include "../ENGINE/ASSERT.h"


#define inRange(a, min, max) ((a >= min) && (a <= max))

// structure that represents button
typedef struct button_t
{
    bool isEnabled;                     //flag if the button is enabled
    bool clicked;                       // flag if the button was clicked in this session
    bool toShow;                        //flag if the button is showed right now
    SDL_Texture *enable_buttonTexture;  //Texture to be presented when the button is enabled
    SDL_Texture *disable_buttonTexture; //Texture to be presented when the button is disabled
    SDL_Rect *location;                 //rectangle to represent button's location
    SDL_Renderer *buttonRenderer;       // Renderer for button
} Button;

/**
 * @brief Creates a button for GUI mode, if an error occured prints a message
 * @param location - reactangle that represents the buttons' place
 * @param buttonRenderer - renderer for the button
 * @param enable_image - path that represents the image of enabled button
 * @param disable_image - path that that represents the image of disabled button
 * @param isEnabled - flag that represents if the button is enabled
 * @param toShow - flag that represents if the button needs to be shown
 * @param isClicked - flag that represents if the button was clicked
 * 
 * @return 
 * source button 
 * otherwise free all memory associated with source button if the button is null
 * 
 */
extern Button *Create_Button(SDL_Rect *location, SDL_Renderer *buttonRenderer, const char *enable_image, const char *disable_image, bool isEnabled, bool toShow, bool isClicked);
/**
 * initializes all given buttons to not clicked mode
 * @param buttonsList - the buttons that need to be initialized to not clicked
 * @param size - the size of the buttonsList
 * the function is void
 */
extern void initButtons(Button **buttonsList, int size);

/**
 * Frees all memory resources associated with the source button. If the
 * source button is NULL, then the function does nothing.
 * @param button - the source button
 */
extern void destroyButton(Button *button);
/**
 * Draws the button according to the button state - is the button enabled or not
 * If the source button is NULL, then the function does nothing.
 *
 * @param button - the source button
 */
extern void drawButton(Button *button);
/**
 * Copies a given rectangle
 * @param src - the source rectangle
 *  @return
 *  NULL, if an allocation error occurred or  src == NULL
 *  copy of the rectangle - otherwise
 */
extern SDL_Rect *copyLocation(SDL_Rect *src);
/**
 * function that prints fail message (ERROR + given string)
 * @param string - the string that represents the error
 * the function is void
 */
extern void failMessage(char *str);

#endif