#ifndef PANEL_H_
#define PANEL_H_

#include "Cell.h"
#define NUM_OF_SET_PANEL_BUTTONS 6

//Window define
#define BOARD_START_X_PANEL 200
#define BOARD_END 572
#define BOARD_BORDER 27
#define CELL_W 68


//Button Dimensions
#define SET_PANEL_BUTTON_X 25
#define SET_PANEL_BUTTON_Y 75
#define SET_PANEL_BUTTON_W 150
#define SET_PANEL_BUTTON_H 50


//represents panel
typedef struct panel{
    Button** buttonList;//Load window button list
    Cell** cellList;
    int buttonCounter;
    int cellCounter;
    bool isSetPanel;
    SDL_Renderer* renderer;
    SDL_Texture* panelTexture;
    SDL_Rect* location;
}Panel;
/**
 * @brief Create a Panel object
 * 
 * @param renderer 
 * @param image 
 * @param location 
 * @param isSetPanel 
 * @return Panel* 
 */
extern Panel* CreatePanel(SDL_Renderer* renderer,SDL_Surface* image, SDL_Rect* location, bool isSetPanel);
/**
 * @brief Create a Game Panel Cells object
 * 
 * @param renderer 
 * @return Cell** 
 */
extern Cell** CreateGamePanelCells(SDL_Renderer* renderer);
/**
 * @brief Create a Set Panel Buttons object
 * 
 * @param renderer 
 * @return Button** 
 */
extern Button** CreateSetPanelButtons(SDL_Renderer* renderer);
/**
 * @brief 
 * 
 * @param panel 
 */
extern void DestroyPanel(Panel* panel);
/**
 * @brief 
 * 
 * @param panel 
 */
extern void DrawPanel(Panel* panel);


#endif