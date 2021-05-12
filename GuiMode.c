#include "GuiMode.h"

int startGuiMode()
{
    //Initialize SDL_videos and Audio_system
    if (SDL_Init(SDL_INIT_VIDEO) | SDL_Init(SDL_INIT_AUDIO))
    {
        printf("ERROR: Unable to initialize GUI: %s\n", SDL_GetError());
        return 1;
    }
    //Initializing sound
    initAudio();


    GuiManager *manager = ManagerCreate();
    // printf("error\n");
    if (manager == NULL)
    {
        SDL_Quit();
        return 0;
    }

    SDL_Event event;
    int quit = 0;
    ManagerDraw(manager);
    playMusic("sound/mainWindowMusic.wav",SDL_MIX_MAXVOLUME);

    pauseAudio();
    while (!quit)
    {
        if (currentState(manager)) // if there's a Quit request
        {
            quit = 1;
            break;
        }

        while (SDL_PollEvent(&event))
        {
            if(manager->activeWin!=GAME_WINDOW){
                // playMusic("GUI/sound/mainWindowMusic.wav",SDL_MIX_MAXVOLUME);
                unPauseAudio();
            }
            else{
                pauseAudio();
            }
            if (currentState(manager))
            {
                quit = 1;
                break;
            }
            if (ManagerHandleEvent(manager, &event) == MANAGER_QUIT)
            {
                quit = 1;
                break;
            }
            ManagerDraw(manager);
        }
    }
    SDL_Delay(500);
    endAudio();
    ManagerDestroy(manager);
    SDL_Quit();
    return 0;
}

int currentState(GuiManager *manager)
{
    assert(manager != NULL);
    if (manager->game->game_status != REGULAR)
    {
        int stat = statusMessage(manager->GameWindow);
        //need to quit
        if (stat == -1 || stat == 2)
        {
            return 1;
        }
        else if (stat == 1)
        {
            //need to quit
            if (gameRestart(manager) == MANAGER_QUIT)
            {
                return 1;
            }
        }
    }
    return 0;
}