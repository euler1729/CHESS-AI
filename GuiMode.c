#include "GuiMode.h"

int startGuiMode()
{
    //Initialize SDL_videos and Audio_system
    if (SDL_Init(SDL_INIT_VIDEO) | SDL_Init(SDL_INIT_AUDIO)){
        printf("ERROR: Unable to initialize GUI: %s\n", SDL_GetError());
        return 1;
    }

    // Iniialing whole game
    GuiManager *manager = ManagerCreate();

    //Initializing sound
    initAudio();

    if (manager == NULL){
        SDL_Quit();
        return 0;
    }

    SDL_Event event;
    int quit = 0;

    ManagerDraw(manager);
    playMusic("sound/mainWindowMusic.wav", SDL_MIX_MAXVOLUME);


    while (!quit)
    {
        // if there's a Quit request
        if (currentState(manager)){
            quit = 1;
            break;
        }
        if(manager->activeWin != GAME_WINDOW){
            playMusic("sound/mainWindowMusic.wav",SDL_MIX_MAXVOLUME/2);
        }
        else{
            playMusic("sound/mainWindowMusic.wav",SDL_MIX_MAXVOLUME/80);
        }

        while (SDL_PollEvent(&event))
        {
            if (currentState(manager)){
                quit = 1;
                break;
            }
            if (ManagerHandleEvent(manager, &event) == MANAGER_QUIT){
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
        if (stat == -1 || stat == 2){
            return 1;
        }
        else if (stat == 1){
            if (gameRestart(manager) == MANAGER_QUIT){
                return 1;
            }
        }
    }
    return 0;
}