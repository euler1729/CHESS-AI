#include "GameWindow.h"


int isClickedOnGame(int x, int y, GameWindow *src)
{
	assert(src != NULL);
	int start_x = GAME_ARGS_INIT;
	int end_x = GAME_ARGS_INIT;
	int start_y = GAME_ARGS_INIT;
	int end_y = GAME_ARGS_INIT;
	int cell = GAME_ARGS_INIT;
	bool show = false;
	cell = pixelToIndex(x, y);

	if (inRange(x, BOARD_START_X, BOARD_END_X) && inRange(y, BOARD_START_Y, BOARD_END_Y)) //checks where there was a click on button
	{
		return cell; //returns on whick button there was a click
	}
	for (int i = 0; i < src->settingPanel->buttonCounter; i++)
	{ //copies parameters needed

		start_x = src->settingPanel->buttonList[i]->location->x;
		end_x = src->settingPanel->buttonList[i]->location->x + src->settingPanel->buttonList[i]->location->w;
		start_y = src->settingPanel->buttonList[i]->location->y;
		end_y = src->settingPanel->buttonList[i]->location->y + src->settingPanel->buttonList[i]->location->h;
		show = src->settingPanel->buttonList[i]->toShow;
		//checks wheer there was a click on button
		if (inRange(x, start_x, end_x) && inRange(y, start_y, end_y) && show)
		{
			return i + (GRID * GRID); //returns on which button there was a click
		}
	}
	return ERROR_FLAG;
}

int pixelToIndex(int x, int y)
{
	int cell_x;
	int cell_y;
	cell_x = xValue(x, BOARD_START_X, BOARD_CELL_DIM); //find the cell's x value using MACRO
	cell_y = yValue(BOARD_END_Y, y, BOARD_CELL_DIM);   //find the cell's y value using MACRO
	return (cell_y * GRID) + cell_x;				   //combine and return index
}

GameWindow *GameWindowdowCreate(CH_Game *game, const char **board_images)
{
	assert(game != NULL);

	GameWindow *win = (GameWindow *)calloc(sizeof(GameWindow), sizeof(char));
	if (win == NULL)
	{
		failMessage("Couldn't create game window!");
		return NULL;
	}
	//Creating an application window with the following settings:
	win->window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (win->window == NULL)
	{
		failMessage("Couldn't create game window!");
		GameWindowdowDestroy(win);
		return NULL;
	}
	win->renderer = SDL_CreateRenderer(win->window, -1, SDL_RENDERER_ACCELERATED);
	if (win->renderer == NULL)
	{
		failMessage("Couldn't create game window!");
		GameWindowdowDestroy(win);
		return NULL;
	}
	//creates board surface
	SDL_Surface *boardSurface = SDL_LoadBMP("././resources/images/Boards/brown.bmp");
	if (boardSurface == NULL)
	{
		failMessage("Couldn't create game window(Surface)!");
		GameWindowdowDestroy(win);
		return NULL;
	}
	SDL_Rect boardLocation = {.x = BOARD_PANEL_START_X, .y = BOARD_PANEL_START_Y, .h = BOARD_HEIGHT, .w = BOARD_WIDTH};

	win->boardPanel = CreatePanel(win->renderer, boardSurface, &boardLocation, false);

	SDL_FreeSurface(boardSurface);
	//creates settings surface
	SDL_Surface *settingSurface = SDL_LoadBMP("./resources/images/pic/graySquare.bmp");
	if (settingSurface == NULL)
	{
		failMessage("Couldn't create game window(settingSurface)!");
		GameWindowdowDestroy(win);
		return NULL;
	}
	SDL_Rect settingLocation = {.x = SET_WINDOW_START_X, .y = SET_WINDOW_START_Y, .h = SET_HEIGHT, .w = SET_WIDTH};
	win->settingPanel = CreatePanel(win->renderer, settingSurface, &settingLocation, true);
	if (win->settingPanel == NULL || win->boardPanel == NULL)
	{
		failMessage("Couldn't create game window(settingSurface)!");
		GameWindowdowDestroy(win);
		return NULL;
	}
	SDL_FreeSurface(settingSurface);
	win->game = game;
	//initializing dragging
	initDragArgs(win);

	int correct = boardUpdate(win->boardPanel, win->game, board_images);
	if (!correct)
	{
		return NULL;
	}
	win->save_the_game = true;
	win->check_printed = false;
	return win;
}

void GameWindowdowDestroy(GameWindow *src)
{
	if (!src)
	{ //if no need to destroy
		return;
	} // destroy fields of struct
	if (src->boardPanel != NULL)
	{
		DestroyPanel(src->boardPanel);
	}
	src->boardPanel = NULL;
	if (src->settingPanel != NULL)
	{
		DestroyPanel(src->settingPanel);
	}
	src->settingPanel = NULL;
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
void initDragArgs(GameWindow *src)
{
	assert(src != NULL);	 //assertion
	src->moving_cell = NULL; // initialize args
	src->to_drag = false;
	src->target_x = GAME_ARGS_INIT;
	src->target_y = GAME_ARGS_INIT;
	src->source_x = GAME_ARGS_INIT;
	src->source_y = GAME_ARGS_INIT;
}
int boardUpdate(Panel *panel, CH_Game *game, const char **board_images)
{
	assert(panel != NULL);
	assert(game != NULL);

	const char *img = NULL;
	SDL_Texture *cellTexture = NULL;
	SDL_Surface *boardSurface = SDL_LoadBMP("./resources/images/Boards/brown.bmp");

	if (boardSurface == NULL)
	{
		failMessage("Couldn't create game window!(boardUpdate)");
		return 0;
	}
	//Creating texture for enabling button
	SDL_Texture *panelTexture = SDL_CreateTextureFromSurface(panel->renderer, boardSurface);
	if (panelTexture == NULL)
	{
		failMessage("Couldn't create game window!(boardUpdate)");
		return 0;
	}
	SDL_FreeSurface(boardSurface);
	for (int i = 0; i < GRID * GRID; ++i)
	{
		SDL_DestroyTexture(panel->cellList[i]->cellTexture);
		//getting the image of the cell
		img = cellToImage(game->gameBoard[i / GRID][i % GRID], board_images);
		if (img != NULL)
		{
			SDL_Surface *cellSurface = SDL_LoadBMP(img);
			if (cellSurface == NULL)
			{
				failMessage("Couldn't create surface(boardUpdate)!");
				return 0;
			}
			cellTexture = SDL_CreateTextureFromSurface(panel->cellList[i]->renderer, cellSurface);
			if (cellTexture == NULL)
			{
				SDL_FreeSurface(cellSurface);
				failMessage("Couldn't create texture(boardUpdate)!");
				return 0;
			}
			SDL_FreeSurface(cellSurface);
		}
		panel->cellList[i]->cellTexture = cellTexture;
		DrawCell(panel->cellList[i]);
		cellTexture = NULL;
	}
	return 1;
}

const char *cellToImage(char ch, const char **board_images)
{
	assert(board_images != NULL);
	switch (ch)
	{			 //switch according to the char that represents the cell
	case PAWN_W: // return the path to the specific image
		return board_images[WHITE_PAWN_IMG];
	case BISHOP_W:
		return board_images[WHITE_BISHOP_IMG];
	case ROOK_W:
		return board_images[WHITE_ROOK_IMG];
	case KNIGHT_W:
		return board_images[WHITE_KNIGHT_IMG];
	case QUEEN_W:
		return board_images[WHITE_QUEEN_IMG];
	case KING_W:
		return board_images[WHITE_KING_IMG];
	case PAWN_B:
		return board_images[BLACK_PAWN_IMG];
	case BISHOP_B:
		return board_images[BLACK_BISHOP_IMG];
	case ROOK_B:
		return board_images[BLACK_ROOK_IMG];
	case KNIGHT_B:
		return board_images[BLACK_KNIGHT_IMG];
	case QUEEN_B:
		return board_images[BLACK_QUEEN_IMG];
	case KING_B:
		return board_images[BLACK_KING_IMG];
	case RED_CELL:
		return board_images[RED_CELL_IMG];
	case YELLOW_CELL:
		return board_images[YELLOW_CELL_IMG];
	case GREEN_CELL:
		return board_images[GREEN_CELL_IMG];
	default:
		return NULL;
	}
}
void GameWindowdowDraw(GameWindow *src)
{
	if (!src)
	{
		return;
	}
	SDL_SetRenderDrawColor(src->renderer, BOARD_R, BOARD_G, BOARD_B, 0);
	SDL_RenderClear(src->renderer);
	DrawPanel(src->settingPanel);
	DrawPanel(src->boardPanel);
	if (src->target_x != GAME_ARGS_INIT && src->to_drag)
	{
		DrawCell(src->moving_cell);
	}
	SDL_RenderPresent(src->renderer);
}
GAME_EVENT GameWindowdowHandleEvent(GameWindow *src, SDL_Event *event, const char **board_images)
{
	if (event == NULL || src == NULL)
	{
		return GAME_EVENT_INVALID_ARGUMENT;
	}
	GAME_EVENT eve;
	int result;
	int res = isClickedOnGame(event->button.x, event->button.y, src); //get event
	switch (event->type)
	{
	case SDL_MOUSEBUTTONUP: //click on something (mouse up)
		eve = buttonUp(src, event, res, board_images);
		if (eve != NO_EVENT)
			return eve;
		break;
	case SDL_WINDOWEVENT: //window close
		if (event->window.event == SDL_WINDOWEVENT_CLOSE)
			return GAME_EVENT_QUIT;
		break;
	case SDL_MOUSEBUTTONDOWN: // button down event
		result = buttonDown(src, event, res, board_images);
		if (result != NO_EVENT)
			return GAME_EVENT_QUIT;
		break;
	case SDL_MOUSEMOTION: // motion event
		Drag(src);
		break;
	default:
		return GAME_EVENT_NONE;
	}
	return GAME_EVENT_NONE;
}
GAME_EVENT buttonUp(GameWindow *src, SDL_Event *event, int res, const char **board_images)
{
	assert(src != NULL);
	int correct;
	int cell_src;

	if (res == EXIT_GAME_BUTTON) //click on exit
	{
		playSound("./resources/sound/click.wav",SDL_MIX_MAXVOLUME);
		SDL_Delay(400);
		return exitMessage(src, true);
	}	
	if (res == MAIN_MENU_GAME_BUTTON) //click on main menu
	{
		playSound("./resources/sound/click.wav",SDL_MIX_MAXVOLUME);
		SDL_Delay(500);
		return exitMessage(src, false);
	}	
	if (res == RESTART_GAME_BUTTON) //click on restart
	{
		playSound("./resources/sound/click.wav",SDL_MIX_MAXVOLUME);
		SDL_Delay(200);
		return GAME_EVENT_RESTART;
	}	
	if (res == LOAD_GAME_BUTTON) //click on load
	{
		playSound("./resources/sound/click.wav",SDL_MIX_MAXVOLUME);
		SDL_Delay(200);
		return GAME_EVENT_LOAD;
	}	
	if (res == SAVE_GAME_BUTTON) //click on save
	{
		playSound("./resources/sound/click.wav",SDL_MIX_MAXVOLUME);
		SDL_Delay(200);
		return GAME_EVENT_SAVE;
	}	
	if (res == UNDO_GAME_BUTTON) //click on undo
	{
		playSound("./resources/sound/click.wav",SDL_MIX_MAXVOLUME);
		SDL_Delay(200);
		return GAME_EVENT_UNDO;
	}	
	if (inRange(res, 0, ((GRID * GRID) - 1)) && (event->button.button == SDL_BUTTON_RIGHT))
	{ //right click
		correct = boardUpdate(src->boardPanel, src->game, board_images);
		if (!correct){
			return GAME_EVENT_QUIT;
		}
		return GAME_EVENT_NONE;
	}
	if (inRange(res, 0, ((GRID * GRID) - 1)) && (event->button.button == SDL_BUTTON_LEFT))
	{ //left click
		cell_src = pixelToIndex(src->source_x + CELL_OFFSET + 1, src->source_y + CELL_OFFSET);
		if (playerFig(src->game, cell_src / GRID, cell_src % GRID))
		{
			correct = Moving(src, cell_src, res, board_images); // make move
			if (correct == QUIT){
				return GAME_EVENT_QUIT; // check if move succeeded
			}
			else if (correct == RESTART){
				return GAME_EVENT_RESTART;
			}
			playSound("./resources/sound/simple_move.wav",SDL_MIX_MAXVOLUME);
			return GAME_EVENT_NONE;
		}
	}
	return NO_EVENT;
}

void Drag(GameWindow *src)
{
	int press;
	int mouse_x;
	int mouse_y;
	int delta_x;
	int delta_y;
	int distance;
	press = SDL_GetMouseState(&mouse_x, &mouse_y); //gets mouse position in window
	if (press && src->to_drag && inRange(mouse_x, BOARD_MIN_X, BOARD_MAX_X) && inRange(mouse_y, BOARD_MIN_Y, BOARD_MAX_Y))
	{ // to drag
		src->target_x = (mouse_x - CELL_OFFSET + 1);
		src->target_y = (mouse_y - CELL_OFFSET);
		delta_x = src->target_x - src->moving_cell->location->x;
		delta_y = src->target_y - src->moving_cell->location->y;
		distance = dist(delta_x, delta_y); // computing distane using MACRO
		if (distance > DRAG_TIME)
		{ //updates the diatance every 5
			src->moving_cell->location->x = src->target_x;
			src->moving_cell->location->y = src->target_y;
		}
	}
	else if (press && src->to_drag)
	{
		src->target_x = borderX(mouse_x);
		src->target_y = borderY(mouse_y);
	}
}




GAME_EVENT exitMessage(GameWindow *src, bool flag)
{
	assert(src != NULL);
	if (src->save_the_game)
	{					  //the game is up to date
		if (flag == true) // flag represents where to go: main menu / quit game
			return GAME_EVENT_QUIT;
		else
			return GAME_EVENT_MAIN_MENU;
	}
	const SDL_MessageBoxButtonData buttons[] = {{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, YES_BUTTONID, "Yes"},
												{SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, NO_BUTTONID, "No"},
												{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,
												 CANCEL_BUTTONID, "Cancel"}};
	const SDL_MessageBoxColorScheme colorScheme =
		{
			{/* .colors (.r, .g, .b) */
			 /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
			 {WHITE, WHITE, WHITE},
			 /* [SDL_MESSAGEBOX_COLOR_TEXT] */
			 {WHITE, BLACK, BLACK},
			 /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
			 {BLACK, BLACK, BLACK},
			 /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
			 {WHITE, WHITE, WHITE},
			 /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
			 {WHITE, BLACK, WHITE}}};
	const SDL_MessageBoxData messageboxdata = {SDL_MESSAGEBOX_INFORMATION, NULL, "Warning", "Do you want to save game before exiting ?", SDL_arraysize(buttons), buttons, &colorScheme};
	int buttonid;
	if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0)
	{
		failMessage("Message box ERROR!");
		return -1;
	}
	if (buttonid == YES_BUTTONID)
	{ // user wants to save game
		if (flag == true)
			return GAME_EVENT_SAVE_EXIT;
		else
			return GAME_EVENT_SAVE_MAIN_MENU;
	}
	else if (buttonid == NO_BUTTONID)
	{ // user doesnt want to save game
		if (flag == true)
			return GAME_EVENT_QUIT;
		else
			return GAME_EVENT_MAIN_MENU;
	}
	else
		return GAME_EVENT_NONE;
}
int buttonDown(GameWindow *src, SDL_Event *event, int res, const char **board_images)
{
	assert(src != NULL);
	int row, col;
	bool check;
	row = (res / GRID); // row of event
	col = (res % GRID); // col of event
	if ((event->button.button == SDL_BUTTON_RIGHT) && inRange(res, 0, ((GRID * GRID) - 1)) && (src->game->difficulty <= DEFAULT_DIFFICULTY) /*&& (src->game->mode == DEFAULT_MODE)*/)
	{															  // get move pre-requesits
		check = getMovesGui(src, row + 1, col + 1, board_images); // get moves
		if (!check)
		{ // if an error occurred
			failMessage("Couldn't allocate memory!");
			return QUIT;
		}
	}
	else if ((event->button.button == SDL_BUTTON_LEFT) && inRange(res, 0, ((GRID * GRID) - 1)))
	{
		if (playerFig(src->game, row, col))
		{ // if player's figure so start to drag
			src->to_drag = true;
			src->moving_cell = src->boardPanel->cellList[res];
		}
		src->source_x = src->boardPanel->cellList[res]->location->x;
		src->source_y = src->boardPanel->cellList[res]->location->y;
	}
	return NO_EVENT;
}
bool getMovesGui(GameWindow *src, int row, int col, const char **board_images)
{
	char **board = (char **)malloc(sizeof(char *) * GRID);
	int correct = 0;
	if (board == NULL)
	{ // allocation error
		printf("ERROR: Couldn't allocate space\n");
		return false;
	}
	for (int i = 0; i < GRID; i++)
	{ //allocates board
		board[i] = (char *)malloc(sizeof(char) * GRID);
		if (board[i] == NULL)
		{ //checks allocation
			for (int j = 0; j < i; j++)
				free(board[j]);
			free(board);
			printf("ERROR: Couldn't allocate space\n");
			return false;
		}
	}
	for (int i = 0; i < GRID; i++)
	{ //init
		for (int j = 0; j < GRID; j++)
			board[i][j] = DEFAULT_CELL; //default char
	}
	getMovesBonus(src->game, row, col, board, true); //invokes the get moves feature
	for (int i = 0; i < GRID; i++)
	{
		for (int j = 0; j < GRID; j++)
		{
			char c = src->game->gameBoard[i][j];
			if (board[i][j] != DEFAULT_CELL)			  //there is a move in get moves board
				src->game->gameBoard[i][j] = board[i][j]; //update
			board[i][j] = c;
		}
	}
	correct = boardUpdate(src->boardPanel, src->game, board_images); //updates the board
	if (!correct)
		return false;
	for (int i = 0; i < GRID; i++)
	{
		for (int j = 0; j < GRID; j++)
			src->game->gameBoard[i][j] = board[i][j];
	}
	for (int i = 0; i < GRID; i++) // frees memory
		free(board[i]);
	free(board);
	return true;
}
int PCMove(GameWindow *src, const char **board_images)
{
	int *move, correct;
	if (src->game->mode == 1 && src->game->currentPlayer == playerPC(src->game))
	{													  //pc move
		move = miniMax(src->game, src->game->difficulty); //get the move
		if (!move)
		{ //checks allocation
			failMessage("Couldn't allocate memory!");
			return 0;
		}
		for (int i = 0; i < CELL_SIZE * CELL_SIZE; i++)
			move[i]++;
		if (setMove(src->game, move) != 0)
		{ //makes the move
			failMessage("Couldn't allocate memory!");
			return 0;
		}
		free(move);
		correct = boardUpdate(src->boardPanel, src->game, board_images); //update the board according to the computer's move
		if (!correct)
			return 0;
		undoUpdate(src->settingPanel, src->game); //updates undo button
		src->save_the_game = false;
		src->check_printed = false; //updates flag fields
	}
	return 1;
}

void undoUpdate(Panel *panel, CH_Game *game)
{
	assert(panel != NULL); // assertions
	assert(game != NULL);
	if (game->undo_hist->actualSize < HISTORY_MIN_LIMIT || game->mode == TWO_PLAYER_MODE) // turn undo to be disable
		panel->buttonList[UNDO_GAME_BUTTON - (GRID * GRID)]->isEnabled = false;
	else // turn undo to be enable
		panel->buttonList[UNDO_GAME_BUTTON - (GRID * GRID)]->isEnabled = true;
	drawButton(panel->buttonList[UNDO_GAME_BUTTON - (GRID * GRID)]);
}

int statusMessage(GameWindow *src)
{
	assert(src != NULL);
	char *string;
	if (src->game->game_status == CHECK)
	{ // if there is a check
		if (!(src->check_printed))
		{							   //check wasnt printed this round
			src->check_printed = true; // check is printed
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Game Status", "There is a CHECK!", NULL);
		}
		return CHECK_FLAG;
	}
	const SDL_MessageBoxButtonData buttons[] = {{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, RESTART_BUTTONID, "Restart"},
												{SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, QUIT_BUTTONID, "Quit"}};
	const SDL_MessageBoxColorScheme colorScheme = {
		{/* .colors (.r, .g, .b) */
		 /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
		 {WHITE, WHITE, WHITE},
		 /* [SDL_MESSAGEBOX_COLOR_TEXT] */
		 {WHITE, BLACK, BLACK},
		 /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
		 {BLACK, BLACK, BLACK},
		 /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
		 {WHITE, WHITE, WHITE},
		 /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
		 {WHITE, BLACK, WHITE}}};
	if (src->game->game_status == CHECKMATE)
	{ //checkmate the winner is
		switchPlayer(src->game);
		if (strcmp(color(src->game->currentPlayer), "white") == 0) //checks for winner
			string = "WHITE player won!";
		else
			string = "BLACK player won!";
		switchPlayer(src->game);
	}
	else if (src->game->game_status == TIE)
	{ //tie
		string = "MATCH TIED!";
	}
	const SDL_MessageBoxData messageboxdata = {SDL_MESSAGEBOX_INFORMATION, NULL, "Game Status", string, SDL_arraysize(buttons), buttons, &colorScheme};
	int buttonid;
	if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0)
	{
		failMessage("Message box ERROR!");
		return ERROR_FLAG;
	}
	return buttonid;
}
int Moving(GameWindow *src, int cell_src, int res, const char **board_images)
{
	assert(src != NULL);												 //assertion
	int move[CELL_SIZE * CELL_SIZE], result, mouse_x, mouse_y, cell_trg; //initializes params
	bool check1, check2;
	SDL_GetMouseState(&mouse_x, &mouse_y);	   //get mouse position
	cell_trg = pixelToIndex(mouse_x, mouse_y); // transfer pixels to cell on board
	move[START_ROW_PLACE] = (cell_src / GRID) + 1;
	move[START_COL_PLACE] = (cell_src % GRID) + 1;
	move[DESTINATION_ROW_PLACE] = (cell_trg / GRID) + 1;
	move[DESTINATION_COL_PLACE] = (cell_trg % GRID) + 1;
	elem *element = transfer(src->game, move);
	if (element == NULL)
	{
		failMessage("Couldn't allocate memory!");
		return QUIT; //quit
	}
	result = isValidMove(src->game, element); //checks if the move is valid
	free(element);
	if (result == ALLOCATION_ERROR)
	{
		failMessage("Couldn't allocate memory!");
		return QUIT; //quit
	}
	else if (result == 1)
	{									   //valid move
		result = setMove(src->game, move); //do the move!
		if (result != 0)
		{
			failMessage("Couldn't allocate memory!");
			return QUIT; //quit
		}
		src->check_printed = false;
		src->save_the_game = false;					   //the saved game is not up to date
		src->moving_cell->location->x = src->source_x; //drag and drop application
		src->moving_cell->location->y = src->source_y;
		check1 = textureUpdate(src->boardPanel->cellList[res], src->moving_cell->img);
		check2 = textureUpdate(src->moving_cell, NULL);
		if ((!check1) || (!check2))
			return QUIT; //quit
		result = boardUpdate(src->boardPanel, src->game, board_images);
		if (!result)
			return QUIT; //quit
		undoUpdate(src->settingPanel, src->game);
		initDragArgs(src);
		if (src->game->game_status != REGULAR)
		{ //check for change in status
			result = statusMessage(src);
			if (result == QUIT_BUTTONID || result == ERROR_FLAG)
				return QUIT; //quit
			else if (result == RESTART_BUTTONID)
				return RESTART; //restart
		}
		if (!PCMove(src, board_images))
			return QUIT; //quit
	}
	else
	{
		src->moving_cell->location->x = src->source_x; //drag and drop init
		src->moving_cell->location->y = src->source_y;
		initDragArgs(src);
	}
	return NONE; //None
}
void GameWindowdowHide(GameWindow *src)
{
	SDL_HideWindow(src->window);
}

void GameWindowdowShow(GameWindow *src)
{
	SDL_ShowWindow(src->window);
}
