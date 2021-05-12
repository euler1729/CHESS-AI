#include "MainMenu.h"


MainWin* CreateMainWindow()
{
	MainWin* res = NULL;
	res = (MainWin*) calloc(sizeof(MainWin), sizeof(char));
	if (res == NULL ) {
		failMessage("Couldn't create main window!");
		return NULL ;
	}
	// Create an application window with the following settings:
	res->mainWindow = SDL_CreateWindow("MAIN MENU",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,MAIN_WINDOW_W,MAIN_WINDOW_H,SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	// Check that the window was successfully created
	if (res->mainWindow == NULL ) {
		failMessage("Couldn't create main window!");
		DestroyMainWindow(res);
		return NULL ;
	}
	res->mainRenderer = SDL_CreateRenderer(res->mainWindow, -1, SDL_RENDERER_ACCELERATED);
	if (res->mainRenderer == NULL ) {
		failMessage("Couldn't create main window!");
		DestroyMainWindow(res);
		return NULL ;
	}



	res->buttonCounter=NUM_OF_MAIN_BUTTONS; //get buttons number
	res->buttonList=(Button**)malloc(sizeof(Button*)*(res->buttonCounter)); //create buttons list
	if (res->buttonList==NULL){
		failMessage("Couldn't create main window!");
		DestroyMainWindow(res);
		return NULL;
	}
	//create rectangles of buttons
	SDL_Rect startR = { .x = MAIN_X, .y = MAIN_Y, .h = MAIN_H, .w = MAIN_W };
	SDL_Rect loadR = { .x = MAIN_X, .y = MAIN_Y+MAIN_H+MAIN_OFFSET, .h = MAIN_H, .w = MAIN_W };
	SDL_Rect exitR = { .x = MAIN_X, .y = MAIN_Y+MAIN_H*2+MAIN_OFFSET*2, .h = MAIN_H, .w = MAIN_W };
	//create buttons
	res->buttonList[MAIN_START_BUTTON]=Create_Button(&startR, res->mainRenderer,
			"./images/pic/newGameFinal.bmp","./images/pic/newGameFinal.bmp", true,true,false);
    // printf("error\n");
	res->buttonList[MAIN_LOAD_BUTTON]=Create_Button(&loadR, res->mainRenderer,
			"./images/pic/LoadGameFinal.bmp", "./images/pic/LoadGameFinal.bmp", true,true,false);
	res->buttonList[MAIN_QUIT_BUTTON]=Create_Button(&exitR, res->mainRenderer,
			"./images/pic/QuitGameFinal.bmp", "./images/pic/QuitGameFinal.bmp", true,true,false);
	for(int i=0;i<NUM_OF_MAIN_BUTTONS;i++){
		if(res->buttonList[i]==NULL){
			failMessage("Couldn't create button!");
			DestroyMainWindow(res);
			return NULL ;
		}
	}
	return res;
}
void DrawMainWindow(MainWin* src)
 {
	assert(src!=NULL);
    //drawing background

	SDL_Surface* mainMenuSurface = SDL_LoadBMP("./images/pic/mainbg.bmp");
	SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(src->mainRenderer, mainMenuSurface);
	SDL_FreeSurface(mainMenuSurface);
	SDL_RenderCopy(src->mainRenderer, bgTexture, NULL, NULL);

    //drawing buttons
	for (int i=0;i<src->buttonCounter;i++){
		drawButton(src->buttonList[i]);
	}
	SDL_RenderPresent(src->mainRenderer);
	SDL_RenderClear(src->mainRenderer);
}

MAIN_EVENT HandleMainWindowEvent(MainWin* src, SDL_Event* event)
{
    if((!event) || (!src)){
        return MAIN_INVALID_ARGUMENT;
    }
    int clk = isClickedOnMain(event->button.x,event->button.y, src);
    switch(event->type)
    {
        case SDL_MOUSEBUTTONUP:
            switch(clk)
            {
                case MAIN_START_BUTTON:
				{
					playSound("sound/click.wav",SDL_MIX_MAXVOLUME);
					return MAIN_START;
				}
                    
                case MAIN_LOAD_BUTTON:{
					playSound("sound/click.wav",SDL_MIX_MAXVOLUME);
					return MAIN_LOAD;
				}
                case MAIN_QUIT_BUTTON:{
					playSound("sound/click.wav",SDL_MIX_MAXVOLUME);
					return MAIN_EXIT;
				}
                    
            }
            break;
        case SDL_WINDOWEVENT:
            if(event->window.event ==SDL_WINDOWEVENT_CLOSE){
                return MAIN_EXIT;
            }
            break;
        default:
            return MAIN_NONE;
    }
    return MAIN_NONE;
}

int isClickedOnMain(int x, int y,MainWin* src) 
{
	assert(src!=NULL);
	int start_x = MAIN_ARGS_INIT;
    int end_x = MAIN_ARGS_INIT;
    int start_y = MAIN_ARGS_INIT;
    int end_y = MAIN_ARGS_INIT;
	bool shown=false; //is button shown

	for(int i=0;i<src->buttonCounter;i++)
    {  //getting location
		start_x = src->buttonList[i]->location->x;
		end_x = src->buttonList[i]->location->x + src->buttonList[i]->location->w;
		start_y = src->buttonList[i]->location->y;
		end_y = src->buttonList[i]->location->y + src->buttonList[i]->location->h;
		shown = src->buttonList[i]->toShow;
        //checking click
		if(inRange(x,start_x,end_x) && inRange(y,start_y,end_y) && shown){
			return i;
        }
    }
	return ERROR_FLAG_MAIN;
}
void DestroyMainWindow(MainWin* src)
{
    if(!src)
    {
        return;
    }
    if(src->buttonList != NULL){
        for(int i=0; i<src->buttonCounter; ++i){
            if(src->buttonList[i]!=NULL){
                destroyButton(src->buttonList[i]);
            }
        }
        free(src->buttonList);
    }
    src->buttonList = NULL;
    if(src->mainRenderer !=NULL){
        SDL_DestroyRenderer(src->mainRenderer);
    }
    src->mainRenderer = NULL;
    if(src->mainWindow !=NULL){
        SDL_DestroyWindow(src->mainWindow);
    }
    src->mainWindow = NULL;
    free(src);
}
void MainWindowHide(MainWin* src) {
	SDL_HideWindow(src->mainWindow);
}

void MainWindowShow(MainWin* src) {
	SDL_ShowWindow(src->mainWindow);
}
