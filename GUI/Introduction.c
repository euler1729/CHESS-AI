#include "Introduction.h"


Intro_Window* IntroWindowCreate()
{
    Intro_Window* win = NULL;
    win = (Intro_Window*) calloc(sizeof(Intro_Window), sizeof(char));
    if(win==NULL){
        failMessage("Couldn't create Introductry window!");
        return NULL;
    }
    //Creating INtrodectory window
    win->windowIntro = SDL_CreateWindow("Introuction", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, INTRO_WINDOW_WIDTH, INTRO_WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    if(win->windowIntro == NULL){
        failMessage("Couldn't create Intoductory window!");
        IntroWindowDestroy(win);
        return NULL;
    }

    //Creating Renderer
    win->RendererIntro = SDL_CreateRenderer(win->windowIntro, -1, SDL_RENDERER_ACCELERATED);
    if(win->RendererIntro == NULL){
        failMessage("Couldn't create Introductory window renderer.");
        IntroWindowDestroy(win);
        return NULL;
    }
    win->introBackButton = (Button*) malloc(sizeof(Button)*sizeof(char));
    if(win->introBackButton==NULL){
        failMessage("Couldn't allocate memory for Introductory window button!");
        IntroWindowDestroy(win);
        return NULL;
    }
    //Initializing Button
    SDL_Rect buttonPos = {.x=BUTTON_POS_X, .y=BUTTON_POS_Y, .h=BUTTON_H, .w=BUTTON_W};
    win->introBackButton = Create_Button(&buttonPos, win->RendererIntro, "./resources/images/pic/Back.bmp", "./resources/images/pic/Back.bmp", true, true, false);
    win->introBackButton->isEnabled = false;
    win->introBackButton->toShow = true;
    if(win->introBackButton==NULL){
        failMessage("Couldn't Craete Introductory Button!");
        IntroWindowDestroy(win);
        return NULL;
    }
    return win;
}

void IntroWindowDestroy(Intro_Window* src)
{
    if(src==NULL){
        return;
    }
    if(src->introBackButton != NULL){
        destroyButton(src->introBackButton);
    }
    if(src->RendererIntro != NULL){
        SDL_DestroyRenderer(src->RendererIntro);
    }
    src->RendererIntro = NULL;
    if(src->windowIntro!=NULL){
        SDL_DestroyWindow(src->windowIntro);
    }
    src->windowIntro = NULL;
    free(src);
}

void IntroWindowDraw(Intro_Window* src)
{
    assert(src!=NULL);
    //Drawing Background
    SDL_Surface* surface = SDL_LoadBMP("./resources/images/pic/mainbg.bmp");
    if(!surface){
        failMessage("Couldn't Load Surface on IntroWindow!\n");
    }
    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(src->RendererIntro, surface);
    if(!bgTexture){
        failMessage("Couldn't Load texture on IntroWindow!\n");
    }
    SDL_FreeSurface(surface);
    SDL_RenderCopy(src->RendererIntro, bgTexture, NULL, NULL);
    drawButton(src->introBackButton);
    SDL_RenderPresent(src->RendererIntro);
}

int isClickedOnBack(int x, int y, Intro_Window* src){
    assert(src!=NULL);
    int start_x = -1;
    int end_x = -1;
    int start_y = -1;
    int end_y = -1;
    bool shown = false;
    
    start_x = src->introBackButton->location->x;
    end_x = src->introBackButton->location->x + src->introBackButton->location->w;
    start_y = src->introBackButton->location->y;
    end_y = src->introBackButton->location->y + src->introBackButton->location->h;
    shown = src->introBackButton->toShow;
    if(inRange(x,start_x,end_x) && inRange(y, start_y, end_y) &&  shown){
        return INTRO_BUTTON_BACK;
    }
    return -1;
}

INTRO_EVENT IntroWindowHandleEvent(Intro_Window* src, SDL_Event *event){
    if( (!event) || (!src)){
        printf("ERROR: INVALID ARGUMENT AT INTROWINDOW\n");
        return INTRO_WINDOW_INVALID_ARGUMENT;
    }
    //recognise the mouse click
    int clk = isClickedOnBack(event->button.x, event->button.y, src);

    switch(event->type)
    {
        case SDL_MOUSEBUTTONUP:
            if(clk == INTRO_BUTTON_BACK){
                playSound("./resources/sound/click2.wav",SDL_MIX_MAXVOLUME);
                return INTRO_BUTTON_BACK;
            }
            break;
        case SDL_WINDOWEVENT://close the game
            if(event->window.event==SDL_WINDOWEVENT_CLOSE){
                return INTRO_WINDOW_EVENT_QUIT;
            }
            break;
        default:
            return INTRO_WINDOW_EVENT_NONE;
    }
    return INTRO_WINDOW_EVENT_NONE;
}

void IntroWindowHide(Intro_Window* src){
    SDL_HideWindow(src->windowIntro);
}
void IntroWindowShow(Intro_Window* src){
    SDL_ShowWindow(src->windowIntro);
}