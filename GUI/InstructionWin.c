#include "InstructionWin.h"


InstrcWin *InstrcWindowCreate()
{
    InstrcWin *win = NULL;
    win = (InstrcWin *)calloc(sizeof(InstrcWin), sizeof(char));
    if (win == NULL)
    {
        failMessage("Couldn't create Instruction window!");
        return NULL;
    }
    //Creating Instrcdectory window
    win->windowInstrc = SDL_CreateWindow("INSTRUCTIONS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, INSTRC_WINDOW_WIDTH, INSTRC_WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    if (win->windowInstrc == NULL)
    {
        failMessage("Couldn't create Instruction window!");
        InstrcWindowDestroy(win);
        return NULL;
    }

    //Creating Renderer
    win->RendererInstrc = SDL_CreateRenderer(win->windowInstrc, -1, SDL_RENDERER_ACCELERATED);
    if (win->RendererInstrc == NULL)
    {
        failMessage("Couldn't create Instrcductory window renderer.");
        InstrcWindowDestroy(win);
        return NULL;
    }
    win->InstrcBackButton = (Button *)malloc(sizeof(Button) * sizeof(char));
    if (win->InstrcBackButton == NULL)
    {
        failMessage("Couldn't allocate memory for Instruction window button!");
        InstrcWindowDestroy(win);
        return NULL;
    }
    //Initializing Button
    SDL_Rect buttonPos = {.x = BUTTON_POS_X, .y = BUTTON_POS_Y, .h = BUTTON_H, .w = BUTTON_W};
    win->InstrcBackButton = Create_Button(&buttonPos, win->RendererInstrc, "./resources/images/pic/Back.bmp", "./resources/images/pic/Back.bmp", true, true, false);
    win->InstrcBackButton->isEnabled = false;
    win->InstrcBackButton->toShow = true;
    if (win->InstrcBackButton == NULL)
    {
        failMessage("Couldn't Craete Instrcductory Button!");
        InstrcWindowDestroy(win);
        return NULL;
    }
    return win;
}

void InstrcWindowDestroy(InstrcWin *src)
{
    ASSERT(src != NULL);
    if (src->InstrcBackButton != NULL)
    {
        destroyButton(src->InstrcBackButton);
    }
    if (src->RendererInstrc != NULL)
    {
        SDL_DestroyRenderer(src->RendererInstrc);
    }
    src->RendererInstrc = NULL;
    if (src->windowInstrc != NULL)
    {
        SDL_DestroyWindow(src->windowInstrc);
    }
    src->windowInstrc = NULL;
    free(src);
}

void InstrcWindowDraw(InstrcWin *src)
{
    ASSERT(src != NULL);
    //Drawing Background
    SDL_Surface *surface = IMG_Load("./resources/images/pic/instrcWin.png");
    if (!surface)
    {
        failMessage("Couldn't Load Surface on InstrcWindow!\n");
    }
    SDL_Texture *bgTexture = SDL_CreateTextureFromSurface(src->RendererInstrc, surface);
    if (!bgTexture)
    {
        failMessage("Couldn't Load texture on InstrcWindow!\n");
    }
    SDL_FreeSurface(surface);
    SDL_RenderCopy(src->RendererInstrc, bgTexture, NULL, NULL);
    drawButton(src->InstrcBackButton);
    SDL_RenderPresent(src->RendererInstrc);
}

int isClickedOnBack(int x, int y, InstrcWin *src)
{
    ASSERT(src != NULL);
    int start_x = -1;
    int end_x = -1;
    int start_y = -1;
    int end_y = -1;
    bool shown = false;

    start_x = src->InstrcBackButton->location->x;
    end_x = src->InstrcBackButton->location->x + src->InstrcBackButton->location->w;
    start_y = src->InstrcBackButton->location->y;
    end_y = src->InstrcBackButton->location->y + src->InstrcBackButton->location->h;
    shown = src->InstrcBackButton->toShow;
    if (inRange(x, start_x, end_x) && inRange(y, start_y, end_y) && shown)
    {
        return INSTRC_BUTTON_BACK;
    }
    return -1;
}

INSTRC_EVENT InstrcWindowHandleEvent(InstrcWin *src, SDL_Event *event)
{
    if ((!event) || (!src))
    {
        printf("ERROR: INVALID ARGUMENT AT InstrcWINDOW\n");
        return INSTRC_WINDOW_INVALID_ARGUMENT;
    }
    //recognise the mouse click
    int clk = isClickedOnBack(event->button.x, event->button.y, src);

    switch (event->type)
    {
    case SDL_MOUSEBUTTONUP:
        if (clk == INSTRC_BUTTON_BACK)
        {
            playSound("./resources/sound/click2.wav", SDL_MIX_MAXVOLUME);
            return INSTRC_BUTTON_BACK;
        }
        break;
    case SDL_WINDOWEVENT: //close the game
        if (event->window.event == SDL_WINDOWEVENT_CLOSE)
        {
            return INSTRC_WINDOW_EVENT_QUIT;
        }
        break;
    default:
        return INSTRC_WINDOW_EVENT_NONE;
    }
    return INSTRC_WINDOW_EVENT_NONE;
}

void InstrcWindowHide(InstrcWin *src)
{
    SDL_HideWindow(src->windowInstrc);
}
void InstrcWindowShow(InstrcWin *src)
{
    SDL_ShowWindow(src->windowInstrc);
}
