#include "Cell.h"

Cell *CreateCell(SDL_Rect *location, SDL_Renderer *renderer, const char *cell_image)
{
    ASSERT((renderer != NULL) && (location != NULL));

    Cell *cell = (Cell *)malloc(sizeof(Cell));
    if (!cell){
        printf("Couldn't Create cell!\n");
        return NULL;
    }

    //Cell field update
    cell->img = cell_image;
    cell->cellTexture = NULL;
    cell->renderer = renderer;
    cell->location = copyLocation(location);
    if (cell->location == NULL){
        DestroyCell(cell);
        return NULL;
    }
    bool check = textureUpdate(cell, cell_image);
    if (!check){
        return NULL;
    }
    return cell;
}
void DestroyCell(Cell *cell)
{
    if (cell == NULL){
        return;
    }
    if (cell->location != NULL){
        free(cell->location);
    }
    cell->location = NULL;
    if (cell->cellTexture != NULL){
        SDL_DestroyTexture(cell->cellTexture);
    }
    cell->cellTexture = NULL;
    cell->renderer = NULL;
    free(cell);
}
bool textureUpdate(Cell *cell, const char *cell_image)
{
    ASSERT(cell != NULL);

    if (cell_image != NULL){
        SDL_Surface *imageSurface = SDL_LoadBMP(cell_image);
        if (imageSurface == NULL){
            printf("ERROR: couldn't create surface!\n");
            return false;
        }
        SDL_Texture *cellTexture = SDL_CreateTextureFromSurface(cell->renderer, imageSurface);
        cell->cellTexture = cellTexture;
    }
    return true;
}
void DrawCell(Cell *cell)
{
    if (cell == NULL){
        return;
    }
    //drawing cell
    SDL_RenderCopy(cell->renderer, cell->cellTexture, NULL, cell->location);
}