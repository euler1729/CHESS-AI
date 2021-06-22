#include "GuiMode.h"

int startGuiMode()
{
    //Initialize SDL_videos and Audio_system
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO| SDL_INIT_TIMER) ){
        printf("ERROR: Unable to initialize GUI: %s\n", SDL_GetError());
        return 1;
    }

    // Iniialing whole game
    GuiManager *manager = ManagerCreate();

    //Initializing sound
    initAudio();

    if (manager == NULL)
    {
        SDL_Quit();
        return 0;
    }

    SDL_Event event;
    int quit = 0;

    ManagerDraw(manager);
    playMusic("./resources/sound/keys-of-moon-white-petals.wav", SDL_MIX_MAXVOLUME);

    while (!quit)
    {
        // if there's a Quit request
        if (currentState(manager)){
            quit = 1;
            break;
        }
        if (manager->activeWin != GAME_WINDOW){
            playMusic("./resources/sound/keys-of-moon-white-petals.wav", SDL_MIX_MAXVOLUME / 2);
        }
        else{
            playMusic("./resources/sound/keys-of-moon-white-petals.wav", SDL_MIX_MAXVOLUME / 100);
        }

        while (SDL_PollEvent(&event)){

            if (ManagerHandleEvent(manager, &event) == MANAGER_QUIT){
                quit = 1;
                break;
            }
            if (currentState(manager)){
                quit = 1;
                break;
            }
            ManagerDraw(manager);
        }
    }
    SDL_Delay(300);
    endAudio();
    ManagerDestroy(manager);
    SDL_Quit();
    return 0;
}

int currentState(GuiManager *manager)
{
    ASSERT(manager != NULL);
    if (manager->game->game_status != REGULAR)
    {
        int stat = statusMessage(manager->GameWindow);
        if (stat == -1 || stat == 2)
        {
            return 1;
        }
        else if (stat == 1)
        {
            if (gameRestart(manager) == MANAGER_QUIT)
            {
                return 1;
            }
        }
    }
    return 0;
}