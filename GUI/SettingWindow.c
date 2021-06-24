#include "SettingWindow.h"


int isClickedOnSetting(int x, int y, SettingsWin *src)
{
	ASSERT(src != NULL);
	int start_x = SET_DRAG_INIT;
	int end_x = SET_DRAG_INIT;
	int start_y = SET_DRAG_INIT;
	int end_y = SET_DRAG_INIT;
	bool shown = false;
	for (int i = 0; i < src->buttonCounter; i++)
	{
		start_x = src->buttonList[i]->location->x;
		end_x = src->buttonList[i]->location->x + src->buttonList[i]->location->w;
		start_y = src->buttonList[i]->location->y;
		end_y = src->buttonList[i]->location->y + src->buttonList[i]->location->h;
		shown = src->buttonList[i]->toShow;
		if (inRange(x, start_x, end_x) && inRange(y, start_y, end_y) && shown){ //check click
			return i;
		}	
	}
	return -1;
}


SettingsWin *SettingsWindowCreate()
{

	SettingsWin *win = NULL;
	bool check = false;

	win = (SettingsWin *)calloc(sizeof(SettingsWin), sizeof(char));
	if (win == NULL)
	{
		failMessage("Couldn't create setting window!");
		return NULL;
	}
	// Creating application window
	win->window = SDL_CreateWindow("GAME SETTINGS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SET_WINDOW_W, SET_WINDOW_H, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (win->window == NULL)
	{
		failMessage("Couldn't create setting window!");
		SettingsWindowDestroy(win);
		return NULL;
	}

	win->renderer = SDL_CreateRenderer(win->window, -1, SDL_RENDERER_ACCELERATED);
	if (win->renderer == NULL)
	{
		failMessage("Couldn't create setting window!");
		SettingsWindowDestroy(win);
		return NULL;
	}
	win->curr_screen = GameModeScreen;
	win->buttonCounter = NUM_OF_SET_BUTTONS;
	win->buttonList = (Button **)malloc(sizeof(Button *) * (win->buttonCounter));
	if (win->buttonList == NULL)
	{
		failMessage("Couldn't create setting window!");
		SettingsWindowDestroy(win);
		return NULL;
	}

	check = setButtonsInit(win);
	if (!check)
	{
		failMessage("Couldn't create setting window!");
		SettingsWindowDestroy(win);
		return NULL;
	}
	return win;
}


bool setButtonsInit(SettingsWin *src)
{
	ASSERT(src != NULL);
	//create rectangles of buttons

	SDL_Rect onePlayer = {.x = PLAYERS_1X_BUTTON, .y = PLAYERS_Y, .h = PLAYERS_H, .w = PLAYERS_W};
	SDL_Rect twoPlayer = {.x = PLAYER_2X_BUTTON, .y = PLAYERS_Y, .h = PLAYERS_H, .w = PLAYERS_W};

	SDL_Rect noob = {.x = DIF_X, .y = DIF_START_Y, .h = DIF_H, .w = DIF_W};
	SDL_Rect easy = {.x = DIF_X, .y = DIF_START_Y + DIF_H + DIF_GAP, .h = DIF_H, .w = DIF_W};
	SDL_Rect moderate = {.x = DIF_X, .y = DIF_START_Y + DIF_H * 2 + DIF_GAP * 2, .h = DIF_H, .w = DIF_W};
	SDL_Rect hard = {.x = DIF_X, .y = DIF_START_Y + DIF_H * 3 + DIF_GAP * 3, .h = DIF_H, .w = DIF_W};
	SDL_Rect white = {.x = WHITE_X_BUTTON, .y = COLOR_BUTTON_Y, .h = COLOR_BUTTON_H, .w = DIF_W};
	SDL_Rect black = {.x = BLACK_X_BUTTON, .y = COLOR_BUTTON_Y, .h = COLOR_BUTTON_H, .w = DIF_W};

	SDL_Rect startR = {.x = OTHERS_BUT_X, .y = OTHERS_BUT_Y, .h = OTHERS_BUT_H, .w = OTHERS_BUT_WIT};
	SDL_Rect nextR = {.x = OTHERS_BUT_X, .y = OTHERS_BUT_Y, .h = OTHERS_BUT_H, .w = OTHERS_BUT_WIT};
	SDL_Rect backR = {.x = 150, .y = OTHERS_BUT_Y, .h = OTHERS_BUT_H, .w = OTHERS_BUT_WIT};

	//create buttons
	src->buttonList[NOOB_BUTTON] = Create_Button(&noob, src->renderer,
												 "./resources/images/pic/noobFinal.bmp", "./resources/images/pic/noob.bmp", false, false, false);
	src->buttonList[EASY_BUTTON] = Create_Button(&easy, src->renderer,
												 "./resources/images/pic/easyFinal.bmp", "./resources/images/pic/easy.bmp", true, false, true);
	src->buttonList[MODERATE_BUTTON] = Create_Button(&moderate, src->renderer,
													 "./resources/images/pic/moderateFinal.bmp", "./resources/images/pic/moderate.bmp", false, false, false);
	src->buttonList[HARD_BUTTON] = Create_Button(&hard, src->renderer,
												 "./resources/images/pic/hardFinal.bmp", "./resources/images/pic/hard.bmp", false, false, false);
	src->buttonList[WHITE_BUTTON] = Create_Button(&white, src->renderer,
												  "./resources/images/pic/k_b_wFinal.bmp", "./resources/images/pic/k_b_w.bmp", true, false, true);
	src->buttonList[BLACK_BUTTON] = Create_Button(&black, src->renderer,
												  "./resources/images/pic/k_b_bFinal.bmp", "./resources/images/pic/k_b_b.bmp", false, false, false);
	src->buttonList[TWO_PLAYER] = Create_Button(&twoPlayer, src->renderer,
												"./resources/images/pic/twoPlayersFinal.bmp", "./resources/images/pic/twoPlayers.bmp", false, true, false);
	src->buttonList[ONE_PLAYER] = Create_Button(&onePlayer, src->renderer,
												"./resources/images/pic/onePlayerFinal.bmp", "./resources/images/pic/onePlayer.bmp", true, true, true);
	src->buttonList[SET_BACK_BUTTON] = Create_Button(&backR, src->renderer,
													 "./resources/images/pic/Back.bmp", "./resources/images/pic/Back.bmp", true, true, false);
	src->buttonList[SET_START_BUTTON] = Create_Button(&startR, src->renderer,
													  "./resources/images/pic/startGame.bmp", "./resources/images/pic/startGame.bmp", false, false, false);
	src->buttonList[SET_NEXT_BUTTON] = Create_Button(&nextR, src->renderer,
													 "./resources/images/pic/next.bmp", "./resources/images/pic/next.bmp", false, true, false);
	for (int i = 0; i < NUM_OF_SET_BUTTONS; i++)
	{
		if (src->buttonList[i] == NULL)
		{ // check if all buttons created
			failMessage("Couldn't create button!");
			SettingsWindowDestroy(src);
			return false;
		}
	}
	return true;
}


void SettingsWindowDestroy(SettingsWin *src)
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
	src->buttonList = NULL;
	if (src->renderer != NULL)
	{
		SDL_DestroyRenderer(src->renderer);
	}
	src->renderer = NULL;
	if (src->window != NULL)
	{
		SDL_DestroyWindow(src->window);
	}
	src->window = NULL;
	free(src);
}


void SettingsWindowDraw(SettingsWin *src)
{
	if (src == NULL)
	{
		return;
	}
	//drawing background
	SDL_Surface *mainMenuSurface = SDL_LoadBMP("./resources/images/pic/mainbg.bmp");
	SDL_Texture *bgTexture = SDL_CreateTextureFromSurface(src->renderer, mainMenuSurface);
	SDL_FreeSurface(mainMenuSurface);
	SDL_RenderCopy(src->renderer, bgTexture, NULL, NULL);

	//drawing buttons
	for (int i = 0; i < src->buttonCounter; i++)
	{
		drawButton(src->buttonList[i]);
	}
	SDL_RenderPresent(src->renderer);
}


void SettingsChangeToGameMode(SettingsWin *src)
{
	if (src == NULL)
	{
		return;
	}

	// changing window to mode window
	SDL_SetRenderDrawColor(src->renderer, SET_R, SET_G, SET_B, 0);
	SDL_RenderClear(src->renderer);
	for (int i = 0; i < src->buttonCounter; i++)
	{ // updated buttons on window
		if (i == ONE_PLAYER || i == TWO_PLAYER || i == SET_BACK_BUTTON || i == SET_NEXT_BUTTON)
			src->buttonList[i]->toShow = true;
		else
			src->buttonList[i]->toShow = false;
		if (inRange(i, SET_BACK_BUTTON, SET_NEXT_BUTTON))
			src->buttonList[i]->clicked = false;
	}
	src->curr_screen = GameModeScreen;
	SDL_RenderPresent(src->renderer);
}


void SettingsChangeToDifficulty(SettingsWin *src)
{
	if (src == NULL)
	{
		return;
	}
	// changing to difficulty window
	SDL_SetRenderDrawColor(src->renderer, SET_R, SET_G, SET_B, 0);
	SDL_RenderClear(src->renderer);

	for (int i = 0; i < src->buttonCounter; i++)
	{
		//updates buttons in window
		if ((i >= NOOB_BUTTON && i < NUM_OF_DIFFICULTIES) || (i == SET_BACK_BUTTON) || (i == SET_NEXT_BUTTON))
			src->buttonList[i]->toShow = true;
		else
			src->buttonList[i]->toShow = false;
		if (inRange(i, SET_BACK_BUTTON, SET_NEXT_BUTTON))
			src->buttonList[i]->clicked = false;
	}
	src->curr_screen = DifficultyScreen;
	SDL_RenderPresent(src->renderer);
}


void SettingsChangeToColor(SettingsWin *src)
{
	if (src == NULL)
	{
		return;
	}
	// changing to color window
	SDL_SetRenderDrawColor(src->renderer, COLOR_R, COLOR_G, COLOR_B, 0);
	SDL_RenderClear(src->renderer);
	for (int i = 0; i < src->buttonCounter; i++)
	{
		// updates buttons in window
		if ((i == BLACK_BUTTON) || (i == WHITE_BUTTON) || (i == SET_BACK_BUTTON) || (i == SET_START_BUTTON))
			src->buttonList[i]->toShow = true;
		else
			src->buttonList[i]->toShow = false;
		if (inRange(i, SET_BACK_BUTTON, SET_NEXT_BUTTON))
			src->buttonList[i]->clicked = false;
	}
	src->curr_screen = ColorScreen;
	SDL_RenderPresent(src->renderer);
}


SETTINGS_EVENT SettingsWindowHandleEvent(SettingsWin *src, SDL_Event *event, CH_Game *game)
{
	if (!event)
	{
		return SETTINGS_EVENT_INVALID_ARGUMENT;
	}
	int clk = isClickedOnSetting(event->button.x, event->button.y, src); //get event
	switch (event->type)
	{
	case SDL_MOUSEBUTTONUP: //click on button
		switch (clk)
		{ // switch on which button was clicked
		case NOOB_BUTTON:
			gameDiffChanges(src, game, NOOB_BUTTON);
			playSound("./resources/sound/click2.wav", SDL_MIX_MAXVOLUME);
			return SETTINGS_EVENT_NONE;
		case EASY_BUTTON:
			gameDiffChanges(src, game, EASY_BUTTON);
			playSound("./resources/sound/click2.wav", SDL_MIX_MAXVOLUME);
			return SETTINGS_EVENT_NONE;
		case MODERATE_BUTTON:
			gameDiffChanges(src, game, MODERATE_BUTTON);
			playSound("./resources/sound/click2.wav", SDL_MIX_MAXVOLUME);
			return SETTINGS_EVENT_NONE;
		case HARD_BUTTON:
			gameDiffChanges(src, game, HARD_BUTTON);
			playSound("./resources/sound/click2.wav", SDL_MIX_MAXVOLUME);
			return SETTINGS_EVENT_NONE;
		case WHITE_BUTTON:
			gameColorChanges(src, game, 1);
			playSound("./resources/sound/click2.wav", SDL_MIX_MAXVOLUME);
			return SETTINGS_EVENT_NONE;
		case BLACK_BUTTON:
			gameColorChanges(src, game, 0);
			playSound("./resources/sound/click2.wav", SDL_MIX_MAXVOLUME);
			return SETTINGS_EVENT_NONE;
		case TWO_PLAYER:
			gameModeChanges(src, game, false);
			playSound("./resources/sound/click2.wav", SDL_MIX_MAXVOLUME);
			return SETTINGS_EVENT_NONE;
		case ONE_PLAYER:
			gameModeChanges(src, game, true);
			playSound("./resources/sound/click2.wav", SDL_MIX_MAXVOLUME);
			return SETTINGS_EVENT_NONE;
		case SET_BACK_BUTTON:
			playSound("./resources/sound/click2.wav", SDL_MIX_MAXVOLUME);
			return SETTINGS_EVENT_BACK;
		case SET_START_BUTTON:
			playSound("./resources/sound/click2.wav", SDL_MIX_MAXVOLUME);
			return SETTINGS_EVENT_START;
		case SET_NEXT_BUTTON:
			playSound("./resources/sound/click2.wav", SDL_MIX_MAXVOLUME);
			return SETTINGS_EVENT_NEXT;
		}
		break;
	case SDL_WINDOWEVENT: //close the window
		if (event->window.event == SDL_WINDOWEVENT_CLOSE)
		{
			return SETTINGS_EVENT_QUIT;
		}
		break;
	default:
		return SETTINGS_EVENT_NONE;
	}
	return SETTINGS_EVENT_NONE;
}


void gameModeChanges(SettingsWin *src, CH_Game *game, bool one_player)
{
	if (one_player)
	{ // updates buttons and game settings
		game->mode = DEFAULT_MODE;
		src->buttonList[ONE_PLAYER]->isEnabled = true;
		src->buttonList[TWO_PLAYER]->isEnabled = false;
	}
	else
	{
		game->mode = TWO_PLAYER_MODE;
		src->buttonList[ONE_PLAYER]->isEnabled = false;
		src->buttonList[TWO_PLAYER]->isEnabled = true;
	}
	src->buttonList[ONE_PLAYER]->clicked = one_player;
	src->buttonList[TWO_PLAYER]->clicked = (!one_player);
	src->buttonList[SET_NEXT_BUTTON]->toShow = one_player;
	src->buttonList[SET_START_BUTTON]->toShow = (!one_player);
}


void gameDiffChanges(SettingsWin *src, CH_Game *game, int diff)
{
	for (int i = 0; i < NUM_OF_DIFFICULTIES; i++)
	{ // updates game difficulty setting and buttons
		if (i != diff)
		{
			src->buttonList[i]->clicked = false;
			src->buttonList[i]->isEnabled = false;
		}
		else
		{
			src->buttonList[i]->clicked = true;
			src->buttonList[i]->isEnabled = true;
			// game->difficulty = i*2;
			if (diff == 3)
			{
				game->difficulty = 5;
			}
			else if (diff == 2)
			{
				game->difficulty = 4;
			}
			else if (diff == 1)
			{
				game->difficulty = 3;
			}
			else
			{
				game->difficulty = 2;
			}
		}
	}
}


void gameColorChanges(SettingsWin *src, CH_Game *game, int color)
{
	ASSERT(src != NULL);
	ASSERT(game != NULL);
	game->user_color = color; //updates user color
	src->buttonList[WHITE_BUTTON]->clicked = color;
	src->buttonList[BLACK_BUTTON]->clicked = (!color);
	src->buttonList[WHITE_BUTTON]->isEnabled = color;
	src->buttonList[BLACK_BUTTON]->isEnabled = (!color);
}


void SettingsWindowHide(SettingsWin *src)
{
	SDL_HideWindow(src->window);
}


void SettingsWindowShow(SettingsWin *src)
{
	SDL_ShowWindow(src->window);
}
