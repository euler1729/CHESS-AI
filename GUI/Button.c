#include "Button.h"

void initButtons(Button **buttonsList, int size)
{
    ASSERT(buttonsList != NULL);
    //initialize all buttons to not clicked
    for (int i = 0; i < size; i++)
    {
        buttonsList[i]->clicked = false;
    }
}

Button *Create_Button(SDL_Rect *location, SDL_Renderer *buttonRenderer, const char *enable_image, const char *disable_image, bool isEnabled, bool toShow, bool isClicked)
{
    ASSERT(buttonRenderer != NULL);
    ASSERT(location != NULL);
    if (enable_image == NULL || disable_image == NULL)
    {
        failMessage("error in image! ");
        return NULL;
    }
    Button *button = (Button *)malloc(sizeof(Button));
    if (!button)
    {
        failMessage("Couldn't Create a Button!\n");
        return NULL;
    }
    // printf("error\n");
    button->isEnabled = isEnabled;
    button->clicked = isClicked;
    button->toShow = toShow;
    button->buttonRenderer = buttonRenderer;
    button->location = NULL;
    button->location = copyLocation(location);
    if (button->location == NULL)
    {
        failMessage("error in button location! ");
        free(button);
        return NULL;
    }
    // printf("error\n");
    //Creating surface for enabling image
    SDL_Surface *loadingSurfaceEnable = SDL_LoadBMP(enable_image);
    if (loadingSurfaceEnable == NULL)
    {
        // printf("error in loading surface enable\n");
        free(button->location);
        free(button);
        failMessage("Couldn't create surface enable button! ");
        return NULL;
    }
    // printf("error\n");
    //Creating surface for disabling image
    SDL_Surface *loadingSurfaceDisable = SDL_LoadBMP(disable_image);
    if (loadingSurfaceDisable == NULL)
    {
        SDL_FreeSurface(loadingSurfaceEnable);
        free(button->location);
        free(button);
        failMessage("Couldn't create surface disable button! ");
        return NULL;
    }
    //Creating texture for enabling Button
    SDL_Texture *enableTexture = SDL_CreateTextureFromSurface(buttonRenderer, loadingSurfaceEnable);
    if (enableTexture == NULL)
    {
        SDL_FreeSurface(loadingSurfaceEnable);
        SDL_FreeSurface(loadingSurfaceDisable);
        free(button->location);
        free(button);
        failMessage("Couldn't create texture! ");
        return NULL;
    }
    SDL_FreeSurface(loadingSurfaceEnable);
    //create texture for disable button
    SDL_Texture *disableTexture = SDL_CreateTextureFromSurface(buttonRenderer, loadingSurfaceDisable);
    if (disableTexture == NULL)
    {
        SDL_FreeSurface(loadingSurfaceEnable);
        SDL_FreeSurface(loadingSurfaceDisable);
        SDL_DestroyTexture(enableTexture);
        free(button->location);
        free(button);
        failMessage("Couldn't create texture! ");
        return NULL;
    }
    //update texture's fields
    SDL_FreeSurface(loadingSurfaceDisable);
    button->disable_buttonTexture = disableTexture;
    button->enable_buttonTexture = enableTexture;
    // printf("error\n");
    return button;
}

void failMessage(char *str)
{
    ASSERT(str != NULL);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR:", str, NULL);
}
SDL_Rect *copyLocation(SDL_Rect *src)
{
    if (!src)
    {
        return NULL;
    }
    SDL_Rect *copy = malloc(sizeof(SDL_Rect));
    if (!copy)
    {
        return NULL;
    }
    copy->h = src->h;
    copy->w = src->w;
    copy->x = src->x;
    copy->y = src->y;
    return copy;
}
void drawButton(Button *button)
{
    if (button == NULL || (!(button->toShow)))
    {
        return;
    }

    if (button->isEnabled) //if button is enabled - draw enabled texture
    {
        SDL_RenderCopy(button->buttonRenderer, button->enable_buttonTexture, NULL, button->location);
    }
    else //draw disabled texture
    {
        SDL_RenderCopy(button->buttonRenderer, button->disable_buttonTexture, NULL, button->location);
    }
}

void destroyButton(Button *button)
{
    if (button == NULL)
    {
        return;
    }
    if (button->location != NULL)
    {
        free((button->location));
    }
    button->location = NULL;

    if (button->disable_buttonTexture != NULL)
    {
        SDL_DestroyTexture(button->disable_buttonTexture);
    }
    button->disable_buttonTexture = NULL;

    if (button->enable_buttonTexture != NULL)
    {
        SDL_DestroyTexture(button->enable_buttonTexture);
    }
    button->enable_buttonTexture = NULL;
    free(button);
}