#include "Sound.h"

//File scope variables to perseist data
static PrivateAudioDevice *iDevice;
static uint32_t iSoundCount;

/**
 * @brief  Wrapper function for playMusic, playSound, playMusicFromMemory, playSoundFromMemory
 * 
 * @param file Provide a filename to load WAV from, or NULL if using FromMemory
 * @param audio Provide an Audio object if copying from memory, or NULL if using a filename
 * @param loop 1 if looping (music), 0 otherwise (sound)
 * @param volume See playSound for explanation
 */
static inline void playAudio(const char *file, Audio *audio, uint8_t loop, int volume);
/**
 * @brief Add a music to the queue, addAudio wrapper for music due to fade
 * 
 * @param root Root of queue
 * @param newAudio   New Audio to add
 */
static void addMusic(Audio *root, Audio *newAudio);
/**
 * @brief Add a sound to the end of the queue
 * 
 * @param root Root of queue
 * @param newAudio New Audio to add 
 */
static void addAudio(Audio *root, Audio *newAudio);
/**
 * @brief  Audio callback function for OpenAudioDevice
 * 
 * @param userdata  Points to linked list of sounds to play, first being a placeholder
 * @param stream Stream to mix sound into
 * @param len  Length of sound to play
 */
static inline void audioCallBack(void *userdata, uint8_t *stream, int len);

void playSound(const char *file, int volume)
{
    playAudio(file, NULL, 0, volume);
}
void playMusic(const char *file, int volume)
{
    playAudio(file, NULL, 1, volume);
}
void playSoundFromMemory(Audio *audio, int volume)
{
    playAudio(NULL, audio, 1, volume);
}
void playMusicFromMemory(Audio *audio, int volume)
{
    playAudio(NULL, audio, 1, volume);
}
void pauseAudio(void)
{
    if (iDevice->audioEnabled)
    {
        SDL_PauseAudioDevice(iDevice->device, 1);
    }
}
void unPauseAudio(void)
{
    if (iDevice->audioEnabled)
    {
        SDL_PauseAudioDevice(iDevice->device, 0);
    }
}
void endAudio(void)
{
    if (iDevice->audioEnabled)
    {
        pauseAudio();
        freeAudio((Audio *)(iDevice->audioSpec).userdata);
        //Closing audio
        SDL_CloseAudioDevice(iDevice->device);
    }
    free(iDevice);
}
void freeAudio(Audio *audio)
{
    Audio *temp;
    while (audio != NULL)
    {
        if (audio->free == 1)
        {
            SDL_FreeWAV(audio->bufferTrue);
        }
        temp = audio;
        audio = audio->next;
        free(temp);
    }
}

void initAudio(void)
{
    Audio *audio;
    iDevice = (PrivateAudioDevice *)calloc(1, sizeof(PrivateAudioDevice));

    iSoundCount = 0;
    if (iDevice == NULL)
    {
        fprintf(stderr, "[%s: %d]Fatal Error: Memory c-allocation error\n", __FILE__, __LINE__);
        return;
    }

    iDevice->audioEnabled = 0;
    if (!(SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO))
    {
        fprintf(stderr, "[%s: %d]Error: SDL_INIT_AUDIO not initialized\n", __FILE__, __LINE__);
        return;
    }

    SDL_memset(&(iDevice->audioSpec), 0, sizeof(iDevice->audioSpec));

    (iDevice->audioSpec).freq = AUDIO_FREQUENCY;
    (iDevice->audioSpec).format = AUDIO_FORMAT;
    (iDevice->audioSpec).channels = AUDIO_CHANNELS;
    (iDevice->audioSpec).samples = AUDIO_SAMPLES;
    (iDevice->audioSpec).callback = audioCallBack;
    (iDevice->audioSpec).userdata = calloc(1, sizeof(Audio));

    audio = (Audio *)(iDevice->audioSpec).userdata;

    if (audio == NULL)
    {
        fprintf(stderr, "[%s: %d]Error: Memory allocation error\n", __FILE__, __LINE__);
        return;
    }

    audio->buffer = NULL;
    audio->next = NULL;

    // audio.userdata = newAudio
    if ((iDevice->device = SDL_OpenAudioDevice(NULL, 0, &(iDevice->audioSpec), NULL, SDL_AUDIO_ALLOW_CHANGES)) == 0)
    {
        fprintf(stderr, "[%s: %d]Warning: failed to open audio device: %s\n", __FILE__, __LINE__, SDL_GetError());
    }
    else
    {
        //setting audio device enabled global flag
        iDevice->audioEnabled = 1;
        //Unpause active audio stream
        unPauseAudio();
    }
}
static inline void audioCallBack(void *userdata, uint8_t *stream, int len)
{
    Audio *audio = (Audio *)userdata;
    Audio *previous = audio;
    int tempLength;
    uint8_t music = 0;

    //Silent the main buffer
    SDL_memset(stream, 0, len);

    //First one i place holder
    audio = audio->next;

    while (audio != NULL)
    {
        if (audio->length > 0)
        {
            if (audio->fade == 1 && audio->loop == 1)
            {
                music = 1;

                if (audio->volume > 0)
                {
                    if (audio->volume - AUDIO_MUSIC_FADE_VALUE < 0)
                    {
                        audio->volume = 0;
                    }
                    else
                    {
                        audio->volume -= AUDIO_MUSIC_FADE_VALUE;
                    }
                }

                else
                {
                    audio->length = 0;
                }
            }
            if (music && audio->loop == 1 && audio->fade == 0)
            {
                tempLength = 0;
            }
            else
            {
                tempLength = ((uint32_t)len > audio->length) ? audio->length : (uint32_t)len;
            }

            SDL_MixAudioFormat(stream, audio->buffer, AUDIO_FORMAT, tempLength, audio->volume);
            audio->buffer += tempLength;
            audio->length -= tempLength;

            previous = audio;
            audio = audio->next;
        }
        else if (audio->loop == 1 && audio->fade == 0)
        {
            audio->buffer = audio->bufferTrue;
            audio->length = audio->lengthTrue;
        }
        else
        {
            previous->next = audio->next;
            if (audio->loop == 0)
            {
                --iSoundCount;
            }
            audio->next = NULL;
            freeAudio(audio);
            audio = previous->next;
        }
    }
}
Audio *createAudio(const char *file, uint8_t loop, int volume)
{
    Audio *newAudio = (Audio *)calloc(1, sizeof(Audio));
    if (!newAudio)
    {
        fprintf(stderr, "[%s: %d]Error: Memory allocation error\n", __FILE__, __LINE__);
        return NULL;
    }

    if (!file)
    {
        fprintf(stderr, "[%s: %d]Warning: filename NULL: %s\n", __FILE__, __LINE__, file);
        return NULL;
    }
    newAudio->next = NULL;
    newAudio->loop = loop;
    newAudio->fade = 0;
    newAudio->free = 1;
    newAudio->volume = volume;

    if (SDL_LoadWAV(file, &(newAudio->audioSpec), &(newAudio->bufferTrue), &(newAudio->lengthTrue)) == NULL)
    {
        fprintf(stderr, "[%s: %d]Warning: failed to open wave file: %s error: %s\n", __FILE__, __LINE__, file, SDL_GetError());
        free(newAudio);
        return NULL;
    }
    newAudio->buffer = newAudio->bufferTrue;
    newAudio->length = newAudio->lengthTrue;
    (newAudio->audioSpec).callback = NULL;
    (newAudio->audioSpec).userdata = NULL;

    return newAudio;
}

static inline void playAudio(const char *file, Audio *audio, uint8_t loop, int volume)
{
    Audio *newAudio;
    //Checks if audio is enabled or not
    if (!iDevice->audioEnabled)
    {
        return;
    }

    //if sounds are under max number of allowed, if not, then don't play
    if (loop)
    {
        if (iSoundCount >= AUDIO_MAX_SOUNDS)
        {
            return;
        }
        else
        {
            ++iSoundCount;
        }
    }
    //Load from file or from memory
    if (file)
    {
        //Creates new music sound with loop
        newAudio = createAudio(file, loop, volume);
    }
    else if (audio)
    {
        newAudio = (Audio *)malloc(sizeof(Audio));
        if (!newAudio)
        {
            fprintf(stderr, "[%s: %d]Fatal Error: Memory allocation error\n", __FILE__, __LINE__);
            return;
        }
        memcpy(newAudio, audio, sizeof(Audio));

        newAudio->volume = volume;
        newAudio->loop = loop;
        newAudio->free = 0;
    }

    else
    {
        fprintf(stderr, "[%s: %d]Warning: filename and Audio parameters NULL\n", __FILE__, __LINE__);
        return;
    }

    //Locking callback function
    SDL_LockAudioDevice(iDevice->device);
    if (loop == 1)
    {
        addMusic((Audio *)(iDevice->audioSpec).userdata, newAudio);
    }
    else
    {
        addAudio((Audio *)(iDevice->audioSpec).userdata, newAudio);
    }
    SDL_UnlockAudioDevice(iDevice->device);
}
static void addMusic(Audio *root, Audio *newAudio)
{
    uint8_t musicFound = 0;
    Audio *rootNext = root->next;

    //Find out any current music, 0,1 or 2 and fade them out
    while (rootNext)
    {
        //Phase out any current music
        if (rootNext->loop == 1 && rootNext->fade == 0)
        {
            if (musicFound)
            {
                rootNext->length = 0;
                rootNext->volume = 0;
            }
            rootNext->fade = 1;
        }
        //Set flag to remove any queued up music in favour of new music
        else if (rootNext->loop == 1 && rootNext->fade == 1)
        {
            musicFound = 1;
        }
        rootNext = rootNext->next;
    }
    addAudio(root, newAudio);
}

static void addAudio(Audio *root, Audio *newAudio)
{
    if (root == NULL)
    {
        return;
    }
    while (root->next)
    {
        root = root->next;
    }
    root->next = newAudio;
}