#ifndef CELL_H_
#define CELL_H_


#include "Button.h"
#include "../ENGINE/ConsoleMode.h"


//Represents Board Cell in the game
typedef struct cell{
    SDL_Texture *cellTexture; //texture to be presented when the cell is enabled
    SDL_Renderer *renderer;   //renderer for cell
    SDL_Rect *location;       //rectangle to represent cell's location
    const char *img;
} Cell;

/**
 * @brief creates cells of the game board
 * 
 * @param location
 * @param renderer
 * @param cell_image
 * @return Cell*
 */
extern Cell *CreateCell(SDL_Rect *location, SDL_Renderer *renderer, const char *cell_image);
/**
 * @brief Destroys the Board cell
 * 
 */
extern void DestroyCell(Cell *cell);
/**
 * @brief Draws the board cell
 * 
 * @param cell 
 */
extern void DrawCell(Cell *cell);
/**
 * @brief Updated the cell textures of game board
 * @param cell
 * @param cell_image
 */
extern bool textureUpdate(Cell *cell, const char *cell_image);

#endif