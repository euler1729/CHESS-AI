#include "GUIManager.h"

GuiManager *ManagerCreate()
{
	GuiManager *mangr = (GuiManager *)malloc(sizeof(GuiManager));
	if (mangr == NULL)
	{
		failMessage("Couldn't create gui manager window!");
		return NULL;
	}

	//creates main window
	mangr->mainWin = CreateMainWindow(); 
	ASSERT(mangr->mainWin!=NULL);

	//initializes gui manager's parameters
	mangr->game = NULL;
	mangr->settingsWin = NULL;
	mangr->loadWin = NULL;
	mangr->GameWindow = NULL;
	mangr->InstrWin = NULL;
	mangr->activeWin = MAIN_WINDOW;
	mangr->prevWin = MAIN_WINDOW;
	mangr->game = gameCreate(HISTORY_SIZE, DEFAULT_DIFFICULTY, DEFAULT_COLOR, DEFAULT_MODE); //create game
	ASSERT(mangr!=NULL);

	//initializes the arrays
	for (int i = 0; i < MAX_OF_SAVED_FILES; i++){ 
		mangr->saved_games[i] = "";
	}
	for (int i = 0; i < NUM_OF_IMAGES; i++){
		mangr->board_images[i] = "";
	}
		
	updateImages(mangr);
	initSaves(mangr); //initialize xml file paths
	return mangr;
}

void updateImages(GuiManager *src)
{
	//init all board images
	//White Pieces
	src->board_images[WHITE_PAWN_IMG] = "./resources/images/pic/WhiteP.bmp";
	src->board_images[WHITE_BISHOP_IMG] = "./resources/images/pic/WhiteB.bmp";
	src->board_images[WHITE_ROOK_IMG] = "./resources/images/pic/WhiteR.bmp";
	src->board_images[WHITE_KNIGHT_IMG] = "./resources/images/pic/WhiteN.bmp";
	src->board_images[WHITE_QUEEN_IMG] = "./resources/images/pic/WhiteQ.bmp";
	src->board_images[WHITE_KING_IMG] = "./resources/images/pic/WhiteK.bmp";
	//Black Pieces
	src->board_images[BLACK_PAWN_IMG] = "./resources/images/pic/BlackP.bmp";
	src->board_images[BLACK_BISHOP_IMG] = "./resources/images/pic/BlackB.bmp";
	src->board_images[BLACK_ROOK_IMG] = "./resources/images/pic/BlackR.bmp";
	src->board_images[BLACK_KNIGHT_IMG] = "./resources/images/pic/BlackN.bmp";
	src->board_images[BLACK_QUEEN_IMG] = "./resources/images/pic/BlackQ.bmp";
	src->board_images[BLACK_KING_IMG] = "./resources/images/pic/BlackK.bmp";
	//Red-YELLOW-GREED cells
	src->board_images[RED_CELL_IMG] = "./resources/images/pic/redCell.bmp";
	src->board_images[YELLOW_CELL_IMG] = "./resources/images/pic/yellowCell.bmp";
	src->board_images[GREEN_CELL_IMG] = "./resources/images/pic/greenCell.bmp";
}

void initSaves(GuiManager *src)
{
	//init all save paths
	src->saved_games[FILE_SLOT_1] = "./resources/saved_games/file_0.xml";
	src->saved_games[FILE_SLOT_2] = "./resources/saved_games/file_1.xml";
	src->saved_games[FILE_SLOT_3] = "./resources/saved_games/file_2.xml";
	src->saved_games[FILE_SLOT_4] = "./resources/saved_games/file_3.xml";
	src->saved_games[FILE_SLOT_5] = "./resources/saved_games/file_4.xml";
}

void ManagerDestroy(GuiManager *src)
{
	if (!src)
	{ // no need to destroy
		return;
	} // destroy all struct fields
	if (src->GameWindow != NULL)
		GameWindowdowDestroy(src->GameWindow);
	src->GameWindow = NULL;
	if (src->loadWin != NULL)
		LoadWindowDestroy(src->loadWin);
	src->loadWin = NULL;
	if (src->settingsWin != NULL)
		SettingsWindowDestroy(src->settingsWin);
	src->settingsWin = NULL;
	if (src->mainWin != NULL)
		DestroyMainWindow(src->mainWin);
	src->mainWin = NULL;
	if (src->game != NULL)
		gameDestroy(src->game);
	if(src->InstrWin != NULL)
		InstrcWindowDestroy(src->InstrWin);
	free(src);
}

void ManagerDraw(GuiManager *src)
{
	ASSERT(src != NULL);
	//draws according to the active window
	if (src->activeWin == MAIN_WINDOW)
		DrawMainWindow(src->mainWin);
	else if (src->activeWin == GAME_WINDOW)
		GameWindowdowDraw(src->GameWindow);
	else if (src->activeWin == SETTINGS_WINDOW)
		SettingsWindowDraw(src->settingsWin);
	else if (src->activeWin == INSTRC_WINDOW)
		InstrcWindowDraw(src->InstrWin);
	else if(src->activeWin == LOAD_WINDOW)
		LoadWindowDraw(src->loadWin);
	else{
		printf("error in ManagerDraw\n");
	}
}


MANAGER_EVENT handleManagerDueToMainEvent(GuiManager *src, MAIN_EVENT event)
{
	ASSERT(src != NULL);
	if (event == MAIN_START)
	{ //if clicked on start
		MainWindowHide(src->mainWin);
		if (src->settingsWin != NULL)
			SettingsWindowDestroy(src->settingsWin);
		src->settingsWin = SettingsWindowCreate(); //go to settings window
		if (src->settingsWin == NULL)
		{
			failMessage("Couldn't create settings window!");
			return MANAGER_QUIT; //quit game
		}
		src->activeWin = SETTINGS_WINDOW; //set active window to settings window
		SettingsWindowShow(src->settingsWin);
	}
	if (event == MAIN_LOAD)
	{ //if clicked on load
		MainWindowHide(src->mainWin);
		src->prevWin = MAIN_WINDOW;
		if (src->loadWin != NULL){
			LoadWindowDestroy(src->loadWin);
		}
		src->loadWin = LoadWindowCreate(num_of_saved_files(src)); //go to load window
		if (src->loadWin == NULL)
		{
			failMessage("Couldn't create load window!");
			return MANAGER_QUIT; //quit game
		}
		src->activeWin = LOAD_WINDOW; //set active window to load window
		LoadWindowShow(src->loadWin);
	}
	if(event == MAIN_INSTRC_WINDOW)
	{
		MainWindowHide(src->mainWin);
		src->prevWin = MAIN_WINDOW;

		if(src->InstrWin==NULL){
			src->InstrWin = InstrcWindowCreate();
		}
		
		if(src->InstrWin==NULL){
			failMessage("Couldn't Create intro window in GUIMANAGER!\n");
			return MANAGER_QUIT;
		}
		src->activeWin = INSTRC_WINDOW;
		InstrcWindowShow(src->InstrWin);
	}
	if (event == MAIN_EXIT)
	{ //quit game if clicked on exit
		return MANAGER_QUIT;
	}
	return MANAGER_NONE;
}
MANAGER_EVENT HandleEventDueToInstrWindow(GuiManager* src, INSTRC_EVENT event)
{
	ASSERT(src!=NULL);
	if(event == INSTRC_BUTTON_BACK)
	{
		InstrcWindowHide(src->InstrWin);
		MainWindowShow(src->mainWin);
		src->activeWin = MAIN_WINDOW;
		src->prevWin = INSTRC_WINDOW;
		return MANAGER_NONE;
	}
	if(event == INSTRC_WINDOW_EVENT_QUIT){
		return MANAGER_QUIT;
	}
	return MANAGER_NONE;
}


MANAGER_EVENT handleManagerDueToLoadEvent(GuiManager *src, LOAD_EVENT event)
{
	ASSERT(src != NULL);
	int correct;
	if (event == lOAD_EVENT_BACK)
	{ //if clicked on back
		LoadWindowHide(src->loadWin);
		if (src->prevWin == MAIN_WINDOW)
		{
			MainWindowShow(src->mainWin); //show main window
			src->activeWin = MAIN_WINDOW;
		}
		else
		{																					  //back from game board window
			correct = boardUpdate(src->GameWindow->boardPanel, src->game, src->board_images); // updates board
			if (!correct)
				return MANAGER_QUIT;
			GameWindowdowShow(src->GameWindow);
			src->activeWin = GAME_WINDOW;
		}
		src->prevWin = LOAD_WINDOW;
		return MANAGER_NONE;
	}
	if (event == lOAD_EVENT_lOAD)
	{ // load button was clicked
		return loadEvent(src);
	}
	if (event == LOAD_EVENT_QUIT) //if clicked on quit
		return MANAGER_QUIT;
	return MANAGER_NONE;
}

MANAGER_EVENT loadEvent(GuiManager *src)
{
	ASSERT(src != NULL);
	int correct = 0;
	for (int i = SLOT_BUTTONS_START_INDEX; i < src->loadWin->buttonCounter; i++)
	{
		if (src->loadWin->buttonList[i - SLOT_BUTTONS_START_INDEX]->clicked == true)
		{
			FILE *xml = fopen(src->saved_games[i - SLOT_BUTTONS_START_INDEX], "r"); //open the file
			if (!xml)
			{ //no file
				failMessage("File doesn't exist or cannot be opened");
				break;
			}
			CH_Game *loadgame = fileToGame(xml);
			if (loadgame == NULL)
			{ //error occurred
				failMessage("File doesn't exist or cannot be opened");
				break;
			}
			gameAssign(loadgame, src->game); //copies the loaded game to the game that the console has
			gameDestroy(loadgame);			 //destroys the load game
			ArrayListClear(src->game->undo_hist);
			if (ferror(xml))
			{ //if there was an error
				failMessage("File doesn't exist or cannot be opened");
				break;
			}
			else if (fclose(xml) != 0)
			{ //close the file - if the close of the file failed
				failMessage("File doesn't exist or cannot be opened");
				break;
			}
			LoadWindowHide(src->loadWin);
			src->prevWin = LOAD_WINDOW;
			if (src->GameWindow == NULL)
			{ //create game windo
				src->GameWindow = GameWindowdowCreate(src->game, src->board_images);
				if (src->GameWindow == NULL)
				{ //checks allocation
					failMessage("Couldn't create gui manager window!");
					return MANAGER_QUIT;
				}
			}
			else
			{ //update game window
				correct = boardUpdate(src->GameWindow->boardPanel, src->game, src->board_images);
				if (!correct)
					return MANAGER_QUIT;
				undoUpdate(src->GameWindow->settingPanel, src->game); // upadtes undo button
				src->GameWindow->save_the_game = true;				  // game is up to date
				src->GameWindow->check_printed = false;				  // check was not printed yet
			}
			GameWindowdowShow(src->GameWindow); //show game window
			src->activeWin = GAME_WINDOW;
			if (!PCMove(src->GameWindow, src->board_images)) // computer move if needed
				return MANAGER_QUIT;
			break;
		}
	}
	return MANAGER_NONE;
}

MANAGER_EVENT handleManagerDueToGameEvent(GuiManager *src, GAME_EVENT event)
{
	if (event == GAME_EVENT_NONE || src == NULL){
		ManagerDraw(src);
		return MANAGER_NONE;
	}
		
	if (event == GAME_EVENT_QUIT){//clicked on quit
		return MANAGER_QUIT;
	}	
	if (event == GAME_EVENT_RESTART){ //clicked on quit
		return gameRestart(src);
	}	
	if ((event == GAME_EVENT_MAIN_MENU) || (event == GAME_EVENT_SAVE_MAIN_MENU))
	{
		if (event == GAME_EVENT_SAVE_MAIN_MENU) //if wanted to save before going to main menu
		{
			saveGameGui(src);//save game
		}	
		GameWindowdowHide(src->GameWindow);
		src->prevWin = MAIN_WINDOW;
		MainWindowShow(src->mainWin); //show main window
		src->activeWin = MAIN_WINDOW;
		CH_Game *new_game = gameCreate(HISTORY_SIZE, DEFAULT_DIFFICULTY, DEFAULT_COLOR, DEFAULT_MODE); //create new game

		if (new_game == NULL){
			failMessage("Couldn't allocate memory in guimanager line 312!");
			return MANAGER_QUIT;
		}

		gameAssign(new_game, src->game); // copies game params
		gameDestroy(new_game);
		ArrayListClear(src->game->undo_hist); // clears history
		return MANAGER_NONE;
	}
	if (event == GAME_EVENT_LOAD)
	{
		GameWindowdowHide(src->GameWindow);
		int num = num_of_saved_files(src); //get num of saved files
		if (src->loadWin == NULL)
		{															  // no load window
			src->loadWin = LoadWindowCreate(num_of_saved_files(src)); //create load window
			if (src->loadWin == NULL)
			{
				failMessage("Couldn't allocate memory gui manager line 329!");
				return MANAGER_QUIT;
			}
		}
		else
		{
			initButtons(src->loadWin->buttonList, src->loadWin->buttonCounter); //initialize buttons
			src->loadWin->buttonList[LOAD_LOAD_BUTTON]->isEnabled = false;
		}
		for (int i = 0; i < num; i++)
		{ // initialize buttons
			src->loadWin->buttonList[i]->isEnabled = false;
			src->loadWin->buttonList[i]->toShow = true;
		}

		LoadWindowDraw(src->loadWin);
		src->prevWin = GAME_WINDOW;
		LoadWindowShow(src->loadWin); //show load window
		src->activeWin = LOAD_WINDOW;
		return MANAGER_NONE;
	}
	if (event == GAME_EVENT_SAVE || event == GAME_EVENT_SAVE_EXIT)
	{
		saveGameGui(src); //save game
		src->GameWindow->save_the_game = true;
		if (event == GAME_EVENT_SAVE_EXIT) // need to save and then exit
			return MANAGER_QUIT;
		return MANAGER_NONE;
	}
	if (event == GAME_EVENT_UNDO && src->game->difficulty<4 && src->game->mode==DEFAULT_MODE && src->game->undo_hist->actualSize>1){
		undoGameGui(src); //undo move
		undoGameGui(src);
	}
	if( (event==GAME_EVENT_UNDO && src->game->difficulty>3 ) || (src->game->mode!=DEFAULT_MODE)){
		playSound("./resources/sound/wrong.wav",SDL_MIX_MAXVOLUME);
		printf("Undo is unavailable for MODERATE and HARD level.\n");
	}

	return MANAGER_NONE;
}

MANAGER_EVENT handleManagerDueToSetEvent(GuiManager *src, SETTINGS_EVENT event)
{
	ASSERT(src != NULL);
	if (event == SETTINGS_EVENT_NONE)
	{ //no event
		return MANAGER_NONE;
	}
	if (event == SETTINGS_EVENT_BACK)
	{							//clicked on back
		drawSetPrevScreen(src); //go to back settings sub screen
		return MANAGER_NONE;
	}
	else if (event == SETTINGS_EVENT_NEXT)
	{							//clicked on next
		drawSetNextScreen(src); //go to next settings sub screen
		return MANAGER_NONE;
	}
	else if (event == SETTINGS_EVENT_START)
	{
		//clicked on start
		SettingsWindowHide(src->settingsWin);
		if (src->GameWindow != NULL){
			GameWindowdowDestroy(src->GameWindow);
		}
		//go to game window
		src->GameWindow = GameWindowdowCreate(src->game, src->board_images);
		if (src->GameWindow == NULL){
			failMessage("Couldn't create game window!");
			return MANAGER_QUIT;
		}
		GameWindowdowShow(src->GameWindow);
		
		src->activeWin = GAME_WINDOW;
		ManagerDraw(src);
		// computer makes move if needed
		if (!PCMove(src->GameWindow, src->board_images)) 
			return MANAGER_QUIT;
	}
	else if (event == SETTINGS_EVENT_QUIT)
	{ //clicked on quit
		return MANAGER_QUIT;
	}
	return MANAGER_NONE;
}

void drawSetNextScreen(GuiManager *src)
{
	ASSERT(src != NULL);
	if (src->settingsWin->curr_screen == GameModeScreen)
	{												  //if the user's curr screen is game mode
		SettingsChangeToDifficulty(src->settingsWin); //go to difficulty screen
		src->settingsWin->curr_screen = DifficultyScreen;
	}
	else if (src->settingsWin->curr_screen == DifficultyScreen)
	{											 //if the user's curr screen is difficulty
		SettingsChangeToColor(src->settingsWin); //go to color screen
		src->settingsWin->curr_screen = ColorScreen;
	}
}

void drawSetPrevScreen(GuiManager *src)
{
	ASSERT(src != NULL);
	if (src->settingsWin->curr_screen == ColorScreen)			//if user's curr screen is color screen
		SettingsChangeToDifficulty(src->settingsWin);			//go to difficulty screen
	else if (src->settingsWin->curr_screen == DifficultyScreen) //if user's curr screen is difficulty screen
		SettingsChangeToGameMode(src->settingsWin);				//go to settings screen
	else if (src->settingsWin->curr_screen == GameModeScreen)
	{ //if user's curr screen is game mode
		SettingsWindowHide(src->settingsWin);
		MainWindowShow(src->mainWin); //go to main window
		src->activeWin = MAIN_WINDOW;
		src->prevWin = MAIN_WINDOW;
	}
}

MANAGER_EVENT ManagerHandleEvent(GuiManager *src, SDL_Event *event)
{
	ASSERT(src != NULL);
	if (event == NULL)
	{
		return MANAGER_NONE;
	}
	if (src->activeWin == MAIN_WINDOW)
	{														   //active window is main window
		MAIN_EVENT mainEvent = HandleMainWindowEvent(src->mainWin, event); //handle the event
		return handleManagerDueToMainEvent(src, mainEvent);				   //handle the windows due to the event of main window
	}
	else if (src->activeWin == LOAD_WINDOW)
	{																	   //active window is load window
		LOAD_EVENT loadEvent = LoadWindowHandleEvent(src->loadWin, event); //handle the event
		return handleManagerDueToLoadEvent(src, loadEvent);				   //handle the windows due to the event of load window
	}
	else if (src->activeWin == SETTINGS_WINDOW)
	{																							 //active window is settings window
		SETTINGS_EVENT setEvent = SettingsWindowHandleEvent(src->settingsWin, event, src->game); //handle the event
		return handleManagerDueToSetEvent(src, setEvent);										 //handle the windows due to the event of settings window
	}
	else if(src->activeWin == INSTRC_WINDOW)
	{	
		INSTRC_EVENT introEvent = InstrcWindowHandleEvent(src->InstrWin, event);
		return HandleEventDueToInstrWindow(src, introEvent);
	}
	else
	{																								//active window is game window
		GAME_EVENT gameEvent = GameWindowdowHandleEvent(src->GameWindow, event, src->board_images); //handle the event
		return handleManagerDueToGameEvent(src, gameEvent); //handle the windows due to the event of game window
	}
	return MANAGER_NONE;
}

int num_of_saved_files(GuiManager *src)
{
	int counter = 0; // counts how many files are saved
	for (int i = 0; i < MAX_OF_SAVED_FILES; i++)
	{
		if (access(src->saved_games[i], F_OK) != -1)
			counter++;
	}
	return counter;
}

void saveUpdate(GuiManager *src)
{ // updates names and saved files
	int saved_counter = num_of_saved_files(src);
	if (saved_counter == MAX_OF_SAVED_FILES)
	{
		remove(src->saved_games[saved_counter - 1]);
		saved_counter--;
	}
	for (int i = saved_counter; i > 0; i--)
		rename(src->saved_games[i - 1], src->saved_games[i]);
}
XML_MESSAGE saveGameGui(GuiManager *src)
{
	saveUpdate(src);
	XML_MESSAGE msg;
	FILE *xml = fopen(src->saved_games[0], "w"); //opens the game
	if (xml == NULL)
	{ //open failed
		failMessage("File cannot be created or modified!");
		return XML_FAILED;
	}
	msg = gameToFile(src->game, xml);
	if (msg == XML_ALLOC)
		fclose(xml);
	if ((msg == XML_FAILED) || (ferror(xml)))
	{ //game is null or there was an error in the file
		failMessage("File cannot be created or modified!");
		return XML_FAILED;
	}
	if (fclose(xml) != 0)
	{ //closing the file failed
		failMessage("File cannot be created or modified!");
		return XML_FAILED;
	}
	return XML_SUCCESS;
}


MANAGER_EVENT gameRestart(GuiManager *src)
{
	CH_Game *copy = gameCreate(HISTORY_SIZE, src->game->difficulty, src->game->user_color, src->game->mode);
	ASSERT(copy!=NULL);
	gameAssign(copy, src->game); // assigns the new game
	gameDestroy(copy);
	ArrayListClear(src->game->undo_hist);// clears history

	int correct = boardUpdate(src->GameWindow->boardPanel, src->game, src->board_images); //updates board
	if (!correct){
		return MANAGER_QUIT;
	}
		
	undoUpdate(src->GameWindow->settingPanel, src->game); // updates undo button
	src->GameWindow->save_the_game = true;				  // is up do date
	src->GameWindow->check_printed = false;				  // check was not printed yet
	if (!PCMove(src->GameWindow, src->board_images)){	  // computer move if needed
		return MANAGER_QUIT;
	}	
	return MANAGER_NONE;
}


void undoGameGui(GuiManager *src)
{
	ASSERT(src != NULL);
	gameUndoMove(src->game);												//undo the move
	boardUpdate(src->GameWindow->boardPanel, src->game, src->board_images); //update the board
	undoUpdate(src->GameWindow->settingPanel, src->game);
}
