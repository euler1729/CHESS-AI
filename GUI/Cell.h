#ifndef CELL_H_
#define CELL_H_

#include "Button.h"
#include "../ConsoleMode.h"

//Represents Board Cell in the game
typedef struct cell
{
    SDL_Texture *cellTexture; //texture to be presented when the cell is enabled
    SDL_Renderer *renderer;   //renderer for cell
    SDL_Rect *location;       //rectangle to represent cell's location
    const char *img;
} Cell;

/**
 * @param location
 * @param renderer
 * @param cell_image
 * @return Cell*
 */
extern Cell *CreateCell(SDL_Rect *location, SDL_Renderer *renderer, const char *cell_image);
/**
 * @param cell
 */
extern void DestroyCell(Cell *cell);
extern void DrawCell(Cell *cell);
/**
 * @param cell
 * @param cell_image
 */
extern bool textureUpdate(Cell *cell, const char *cell_image);

#endif