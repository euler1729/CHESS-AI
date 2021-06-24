#include "Panel.h"


Panel *CreatePanel(SDL_Renderer *renderer, SDL_Surface *image, SDL_Rect *location, bool isSetPanel)
{
	if ((image == NULL) || (renderer == NULL))
	{
		return NULL;
	}
	Panel *panel = (Panel *)malloc(sizeof(Panel));
	if (panel == NULL)
	{
		printf("Error: couldn't create panel!\n");
		return NULL;
	}
	//Initializing parameters
	panel->buttonCounter = 0;
	panel->cellCounter = 0;
	panel->cellList = NULL;
	panel->buttonList = NULL;
	panel->renderer = renderer;
	panel->location = NULL;
	panel->location = copyLocation(location);

	//Create texture to enable button
	SDL_Texture *panelTexture = SDL_CreateTextureFromSurface(panel->renderer, image);

	if (panelTexture == NULL)
	{
		DestroyPanel(panel);
		printf("ERROR: coundn't create texture!\n");
		return NULL;
	}
	panel->isSetPanel = isSetPanel;
	if (isSetPanel) //Create button for setting panel
	{
		panel->buttonCounter = NUM_OF_SET_PANEL_BUTTONS;
		panel->buttonList = CreateSetPanelButtons(panel->renderer);
		if (panel->buttonList == NULL)
		{
			DestroyPanel(panel);
			return NULL;
		}
	}
	else //Creating button for Board
	{
		panel->cellCounter = GRID * GRID;
		panel->cellList = CreateGamePanelCells(panel->renderer);
		if (panel->cellList == NULL)
		{
			DestroyPanel(panel);
			return NULL;
		}
	}
	panel->panelTexture = panelTexture;
	return panel;
}

Button **CreateSetPanelButtons(SDL_Renderer *renderer)
{
	Button **buttonList = (Button **)malloc(sizeof(Button *) * NUM_OF_SET_PANEL_BUTTONS);

	if (buttonList == NULL)
	{
		printf("Couldn't not create buttons list:%s\n", SDL_GetError());
		return NULL;
	}
	//Create rectangles to represent button
	SDL_Rect restartR = {.x = SET_PANEL_BUTTON_X, .y = SET_PANEL_BUTTON_Y, .h = SET_PANEL_BUTTON_H, .w = SET_PANEL_BUTTON_W};
	SDL_Rect saveR = {.x = SET_PANEL_BUTTON_X, .y = SET_PANEL_BUTTON_Y + SET_PANEL_BUTTON_H + SET_PANEL_BUTTON_X, .h = SET_PANEL_BUTTON_H, .w = SET_PANEL_BUTTON_W};
	SDL_Rect loadR = {.x = SET_PANEL_BUTTON_X, .y = SET_PANEL_BUTTON_Y + SET_PANEL_BUTTON_H * 2 + SET_PANEL_BUTTON_X * 2, .h = SET_PANEL_BUTTON_H, .w = SET_PANEL_BUTTON_W};
	SDL_Rect undoR = {.x = SET_PANEL_BUTTON_X, .y = SET_PANEL_BUTTON_Y + SET_PANEL_BUTTON_H * 3 + SET_PANEL_BUTTON_X * 3, .h = SET_PANEL_BUTTON_H, .w = SET_PANEL_BUTTON_W};
	SDL_Rect mainMenuR = {.x = SET_PANEL_BUTTON_X, .y = SET_PANEL_BUTTON_Y + 4 * SET_PANEL_BUTTON_H + 4 * SET_PANEL_BUTTON_X, .h = SET_PANEL_BUTTON_H, .w = SET_PANEL_BUTTON_W};
	SDL_Rect exitR = {.x = SET_PANEL_BUTTON_X, .y = SET_PANEL_BUTTON_Y + SET_PANEL_BUTTON_H * 5 + SET_PANEL_BUTTON_X * 5, .h = SET_PANEL_BUTTON_H, .w = SET_PANEL_BUTTON_W};

	//buttons create
	buttonList[0] = Create_Button(&restartR, renderer,
								  "./resources/images/pic/restart.bmp", "./resources/images/pic/restart.bmp", true, true,
								  false);
	buttonList[1] = Create_Button(&saveR, renderer,
								  "./resources/images/pic/saveGame.bmp", "./resources/images/pic/saveGame.bmp", true,
								  true, false);
	buttonList[2] = Create_Button(&loadR, renderer,
								  "./resources/images/pic/LoadGameFinal.bmp", "./resources/images/pic/LoadGameFinal.bmp",
								  true, true, false);
	buttonList[3] = Create_Button(&undoR, renderer,
								  "./resources/images/pic/UndoFinal.bmp", "./resources/images/pic/undo.bmp", false,
								  true, false);
	buttonList[4] = Create_Button(&mainMenuR, renderer,
								  "./resources/images/pic/mainMenu.bmp", "./resources/images/pic/mainMenu.bmp", true,
								  true, false);
	buttonList[5] = Create_Button(&exitR, renderer,
								  "./resources/images/pic/QuitGameFinal.bmp", "./resources/images/pic/QuitGameFinal.bmp", true,
								  true, false);

	for (int i = 0; i < NUM_OF_SET_PANEL_BUTTONS; ++i)
	{
		if (buttonList[i] == NULL)
		{
			for (int j = 0; j < i; ++j)
			{
				destroyButton(buttonList[j]);
			}
			free(buttonList);
			printf("Couldn't not create button: %s\n", SDL_GetError());
			return NULL;
		}
	}
	return buttonList;
}

Cell **CreateGamePanelCells(SDL_Renderer *renderer)
{
	int j = 0;
	Cell **cellList = (Cell **)malloc(sizeof(Cell *) * (GRID * GRID));
	if (cellList == NULL)
	{
		printf("Could not create buttons list: %s\n", SDL_GetError());
		return NULL;
	}
	//create rect to represent buttons
	SDL_Rect **rects = (SDL_Rect **)malloc(sizeof(SDL_Rect *) * (GRID * GRID));
	if (rects == NULL)
	{
		return NULL;
	}
	for (int i = 0; i < GRID * GRID; i++)
	{
		rects[i] = (SDL_Rect *)malloc(sizeof(SDL_Rect));
		if (rects[i] == NULL)
		{
			for (int j = 0; j < i; j++)
			{
				free(rects[j]);
			}
			free(rects);
			return NULL;
		}
		if (i % GRID == 0)
		{
			j++;
		}
		rects[i]->x = BOARD_START_X_PANEL + (BOARD_BORDER + (CELL_W * (i % GRID)));
		rects[i]->y = (BOARD_END - (CELL_W * j));
		rects[i]->h = CELL_W + 1;
		rects[i]->w = CELL_W + 2;
	}

	for (int i = 0; i < GRID * GRID; i++)
	{
		cellList[i] = CreateCell(rects[i], renderer, NULL);
		if (cellList[i] == NULL)
		{ //error in init - destroy
			for (int j = 0; j < GRID * GRID; j++)
			{
				free(rects[j]);
			}
			for (int k = 0; k < i; k++)
			{
				DestroyCell(cellList[k]);
			}
			free(cellList);
			free(rects);
			printf("Could not create cells: %s\n", SDL_GetError());
			return NULL;
		}
	}
	for (int j = 0; j < GRID * GRID; j++)
	{
		free(rects[j]);
	}
	free(rects);
	return cellList;
}

void DrawPanel(Panel *panel)
{
	if (panel == NULL)
	{
		return;
	}
	if (panel->isSetPanel)
	{ //if button is enabled - draw enabled texture
		SDL_RenderCopy(panel->renderer, panel->panelTexture, NULL, panel->location);
		for (int i = 0; i < panel->buttonCounter; i++)
		{
			drawButton(panel->buttonList[i]);
		}
	}
	else
	{ //draw disabled texture
		SDL_RenderCopy(panel->renderer, panel->panelTexture, NULL, panel->location);
		for (int i = 0; i < panel->cellCounter; i++)
		{
			DrawCell(panel->cellList[i]);
		}
	}
}

void DestroyPanel(Panel *panel)
{
	if (panel == NULL)
	{
		return;
	}

	if (panel->buttonList != NULL)
	{
		for (int i = 0; i < panel->buttonCounter; i++)
		{
			if (panel->buttonList[i] != NULL)
			{
				destroyButton(panel->buttonList[i]);
			}
		}
		free(panel->buttonList);
	}
	panel->buttonList = NULL;
	if (panel->cellList != NULL)
	{
		for (int i = 0; i < panel->cellCounter; i++)
		{
			if (panel->cellList[i] != NULL)
			{
				DestroyCell(panel->cellList[i]);
			}
		}
		free(panel->cellList);
	}
	panel->cellList = NULL;
	if (panel->location != NULL)
	{
		free(panel->location);
	}
	panel->location = NULL;
	if (panel->panelTexture != NULL)
	{
		SDL_DestroyTexture(panel->panelTexture);
	}
	panel->panelTexture = NULL;
	free(panel);
}