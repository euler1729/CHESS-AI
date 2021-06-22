#include "LoadWindow.h"

int isClickedOnLoad(int x, int y, LoadWin *src)
{
	ASSERT(src != NULL);
	
	int start_x = LOAD_ARGS_INIT;
	int end_x = LOAD_ARGS_INIT;
	int start_y = LOAD_ARGS_INIT;
	int end_y = LOAD_ARGS_INIT;

	bool show = false;

	for (int i = 0; i < src->buttonCounter; ++i)
	{
		start_x = src->buttonList[i]->location->x;
		end_x = start_x + src->buttonList[i]->location->w;
		start_y = src->buttonList[i]->location->y;
		end_y = start_y + src->buttonList[i]->location->h;
		show = src->buttonList[i]->toShow;
		if (inRange(x, start_x, end_x) && inRange(y, start_y, end_y) && show)
		{
			return i;
		}
	}
	return ERROR_FLAG_LOAD;
}


LoadWin *LoadWindowCreate(int num_of_saved_games)
{
	LoadWin *win = NULL;
	bool check = false;

	win = (LoadWin *)calloc(sizeof(LoadWin), sizeof(char));
	if (!win)
	{
		failMessage("Couldn't create load window!");
		return NULL;
	}
	//Creating an application window with the following settings
	win->windowLoad = SDL_CreateWindow("LOAD MENU", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LOAD_WINDOW_W, LOAD_WINDOW_H, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	if (!win->windowLoad)
	{
		failMessage("Couldn't create load window!");
		LoadWindowDestroy(win);
		return NULL;
	}
	// Creating renderer
	win->rendererLoad = SDL_CreateRenderer(win->windowLoad, -1, SDL_RENDERER_ACCELERATED);
	if (win->rendererLoad == NULL)
	{
		failMessage("Couldn't create load window!");
		LoadWindowDestroy(win);
		return NULL;
	}
	win->buttonCounter = NUM_OF_LOAD_BUTTONS;
	win->buttonList = (Button **)malloc(sizeof(Button *) * (win->buttonCounter));

	if (win->buttonList == NULL)
	{
		failMessage("Couldn't create load window!");
		LoadWindowDestroy(win);
		return NULL;
	}
	// initializes the buttons
	check = loadButtonInit(win, num_of_saved_games);
	if (!check)
	{
		return NULL;
	}
	return win;
}


bool loadButtonInit(LoadWin *win, int num_of_saved_games)
{
	//create rect to represent buttons
	SDL_Rect slot1 = {.x = SLOTS_X_VAL, .y = SLOTS_Y_VAL, .h = SLOTS_H, .w = SLOTS_W};
	SDL_Rect slot2 = {.x = SLOTS_X_VAL, .y = SLOTS_Y_VAL + SLOTS_H + SLOTS_Y_OFFSET, .h = SLOTS_H, .w = SLOTS_W};
	SDL_Rect slot3 = {.x = SLOTS_X_VAL, .y = SLOTS_Y_VAL + SLOTS_H * 2 + SLOTS_Y_OFFSET * 2, .h = SLOTS_H, .w = SLOTS_W};
	SDL_Rect slot4 = {.x = SLOTS_X_VAL, .y = SLOTS_Y_VAL + SLOTS_H * 3 + SLOTS_Y_OFFSET * 3, .h = SLOTS_H, .w = SLOTS_W};
	SDL_Rect slot5 = {.x = SLOTS_X_VAL, .y = SLOTS_Y_VAL + SLOTS_H * 4 + SLOTS_Y_OFFSET * 4, .h = SLOTS_H, .w = SLOTS_W};

	SDL_Rect backR = {.x = BACK_X, .y = OTHER_Y, .h = OTHER_H, .w = OTHER_W};
	SDL_Rect loadR = {.x = LOAD_X, .y = OTHER_Y, .h = OTHER_H, .w = OTHER_W};

	//buttons create
	win->buttonList[SLOT1_BUTTON] = Create_Button(&slot1, win->rendererLoad,
												  "./resources/images/pic/GameSlot1Final.bmp", "./resources/images/pic/GameSlot1.bmp", false, false, false);
	win->buttonList[SLOT2_BUTTON] = Create_Button(&slot2, win->rendererLoad,
												  "./resources/images/pic/GameSlot2Final.bmp", "./resources/images/pic/GameSlot2.bmp", false, false, false);
	win->buttonList[SLOT3_BUTTON] = Create_Button(&slot3, win->rendererLoad,
												  "./resources/images/pic/GameSlot3Final.bmp", "./resources/images/pic/GameSlot3.bmp", false, false, false);
	win->buttonList[SLOT4_BUTTON] = Create_Button(&slot4, win->rendererLoad,
												  "./resources/images/pic/GameSlot4Final.bmp", "./resources/images/pic/GameSlot4.bmp", false, false, false);
	win->buttonList[SLOT5_BUTTON] = Create_Button(&slot5, win->rendererLoad,
												  "./resources/images/pic/GameSlot5Final.bmp", "./resources/images/pic/GameSlot5.bmp", false, false, false);
	win->buttonList[LOAD_BACK_BUTTON] = Create_Button(&backR, win->rendererLoad,
													  "./resources/images/pic/Back.bmp", "./resources/images/pic/Back.bmp", true, true, false);
	win->buttonList[LOAD_LOAD_BUTTON] = Create_Button(&loadR, win->rendererLoad,
													  "./resources/images/pic/LoadGameFinal.bmp", "./resources/images/pic/LoadGame.bmp", false, true, false);
	for (int i = 0; i < num_of_saved_games; i++)
	{
		win->buttonList[i]->isEnabled = false;
		win->buttonList[i]->toShow = true;
	}
	for (int i = 0; i < NUM_OF_LOAD_BUTTONS; i++)
	{
		if (win->buttonList[i] == NULL)
		{
			failMessage("Couldn't create load window!");
			LoadWindowDestroy(win);
			return false;
		}
	}
	return true;
}


void LoadWindowDraw(LoadWin *src)
{
	ASSERT(src != NULL);

	//drawing background
	SDL_Surface *surface = SDL_LoadBMP("./resources/images/pic/mainbg.bmp");
	if (surface == NULL)
	{
		failMessage("Couldn't Load surface!\n");
	}
	SDL_Texture *bgTexture = SDL_CreateTextureFromSurface(src->rendererLoad, surface);
	if (bgTexture == NULL)
	{
		failMessage("Couldn't Load texture!\n");
	}
	SDL_FreeSurface(surface);
	SDL_RenderCopy(src->rendererLoad, bgTexture, NULL, NULL);

	//draw all buttons
	for (int i = 0; i < src->buttonCounter; i++)
	{
		drawButton(src->buttonList[i]);
	}
	SDL_RenderPresent(src->rendererLoad);
}


LOAD_EVENT LoadWindowHandleEvent(LoadWin *src, SDL_Event *event)
{
	if ((!event) || (!src))
	{
		printf("ERROR: INVALID ARGUMENT\n");
		return lOAD_EVENT_INVALID_ARGUMENT;
	}
	int win = isClickedOnLoad(event->button.x, event->button.y, src);
	switch (event->type)
	{
	case SDL_MOUSEBUTTONUP:
		switch (win)
		{ //checks which button was clicked
			case SLOT1_BUTTON:
			{	activateAfterClick(src, lOAD_EVENT_SLOT1);
				playSound("./resources/sound/click2.wav", SDL_MIX_MAXVOLUME);
				return lOAD_EVENT_NONE;
			}
			case SLOT2_BUTTON:
			{	activateAfterClick(src, lOAD_EVENT_SLOT2);
				playSound("./resources/sound/click2.wav", SDL_MIX_MAXVOLUME);
				return lOAD_EVENT_NONE;
			}
			case SLOT3_BUTTON:
			{	activateAfterClick(src, lOAD_EVENT_SLOT3);
				playSound("./resources/sound/click2.wav", SDL_MIX_MAXVOLUME);
				return lOAD_EVENT_NONE;
			}
			case SLOT4_BUTTON:
			{	activateAfterClick(src, lOAD_EVENT_SLOT4);
				playSound("./resources/sound/click2.wav", SDL_MIX_MAXVOLUME);
				return lOAD_EVENT_NONE;
			}
			case SLOT5_BUTTON:
			{	activateAfterClick(src, lOAD_EVENT_SLOT5);
				playSound("./resources/sound/click2.wav", SDL_MIX_MAXVOLUME);
				return lOAD_EVENT_NONE;
			}
			case LOAD_BACK_BUTTON:
			{	playSound("./resources/sound/click2.wav", SDL_MIX_MAXVOLUME);
				return lOAD_EVENT_BACK;
			}
			case LOAD_LOAD_BUTTON:
			{	playSound("./resources/sound/click2.wav", SDL_MIX_MAXVOLUME);
				return lOAD_EVENT_lOAD;
			}
		}
			break;
			case SDL_WINDOWEVENT:
			//checks whether the user tried to close the window
			if (event->window.event == SDL_WINDOWEVENT_CLOSE)
			{
			return LOAD_EVENT_QUIT;
			}
			break;
		default:
			return lOAD_EVENT_NONE;
	}
	return lOAD_EVENT_NONE;
}


void activateAfterClick(LoadWin *src, int event)
{
	for (int i = SLOT_BUTTONS_START_INDEX; i < src->buttonCounter; i++)
	{ // activates the button was clicked
		if (event == i)
		{
			if (src->buttonList[i - SLOT_BUTTONS_START_INDEX]->toShow)
			{
				src->buttonList[i - SLOT_BUTTONS_START_INDEX]->clicked = true;
				src->buttonList[LOAD_LOAD_BUTTON]->isEnabled = true;
				drawButton(src->buttonList[LOAD_LOAD_BUTTON]);
				break;
			}
		}
	}
	for (int i = SLOT_BUTTONS_START_INDEX; i < src->buttonCounter; i++)
	{ // Deactivates the other buttons
		if (event != i)
		{
			src->buttonList[i - SLOT_BUTTONS_START_INDEX]->clicked = false;
			src->buttonList[i - SLOT_BUTTONS_START_INDEX]->isEnabled = false;
		}
		else
		{
			src->buttonList[i - SLOT_BUTTONS_START_INDEX]->clicked = true;
			src->buttonList[i - SLOT_BUTTONS_START_INDEX]->isEnabled = true;
		}
	}
}


void LoadWindowHide(LoadWin *src)
{
	ASSERT(src != NULL);
	SDL_HideWindow(src->windowLoad);
}


void LoadWindowShow(LoadWin *src)
{
	ASSERT(src != NULL);
	SDL_ShowWindow(src->windowLoad);
}


void LoadWindowDestroy(LoadWin *src)
{
	if (!src)
	{
		return;
	}
	if (src->buttonList != NULL)
	{
		for (int i = 0; i < src->buttonCounter; i++)
		{
			if (src->buttonList[i] != NULL)
				destroyButton(src->buttonList[i]);
		}
		free(src->buttonList);
	}
	// destroys all struct fields
	src->buttonList = NULL;
	if (src->rendererLoad != NULL)
	{
		SDL_DestroyRenderer(src->rendererLoad);
	}
	src->rendererLoad = NULL;
	if (src->windowLoad != NULL)
	{
		SDL_DestroyWindow(src->windowLoad);
	}
	src->windowLoad = NULL;
	free(src);
}