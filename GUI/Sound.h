#ifndef SOUND_H_
#define SOUND_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>



// SDL_AudioFormat of sound
#define AUDIO_FORMAT AUDIO_S16LSB
// Frequency of sound
#define AUDIO_FREQUENCY 44100
//Audio Channel 
#define AUDIO_CHANNELS 2
// Specifies a unit of audio data will be used at a time. Must be a power of 2
#define AUDIO_SAMPLES 4096
//Maximum Number of Sound that can be in the audio queue at a time. Stops too mush mixing
#define AUDIO_MAX_SOUNDS 25
//The rate at which the volume fades when misics transition. The higher number indicates music fades faster
#define AUDIO_MUSIC_FADE_VALUE 2
/* Flags OR'd together, which specify how SDL should behave when a device cannot offer a specific feature
 * If flag is set, SDL will change the format in the actual audio file structure (as opposed to gDevice->want)
 *
 * Note: If you're having issues with Emscripten / EMCC play around with these flags
 *
 * 0                                    Allow no changes
 * SDL_AUDIO_ALLOW_FREQUENCY_CHANGE     Allow frequency changes (e.g. AUDIO_FREQUENCY is 48k, but allow files to play at 44.1k
 * SDL_AUDIO_ALLOW_FORMAT_CHANGE        Allow Format change (e.g. AUDIO_FORMAT may be S32LSB, but allow wave files of S16LSB to play)
 * SDL_AUDIO_ALLOW_CHANNELS_CHANGE      Allow any number of channels (e.g. AUDIO_CHANNELS being 2, allow actual 1)
 * SDL_AUDIO_ALLOW_ANY_CHANGE           Allow all changes above
 */
#define SDL_AUDIO_ALLOW_CHANGES SDL_AUDIO_ALLOW_ANY_CHANGE


// Queue for all loaded sound
typedef struct audio{

    uint32_t length;
    uint32_t lengthTrue;
    uint8_t* bufferTrue;
    uint8_t* buffer;
    uint8_t loop;
    uint8_t fade;
    uint8_t free;
    uint8_t volume;
    SDL_AudioSpec audioSpec;
    struct audio *next;
}Audio;

// Global Sound Device
typedef struct privateAudioDevice{
    
    SDL_AudioDeviceID device;
    SDL_AudioSpec audioSpec;
    uint8_t audioEnabled;

}PrivateAudioDevice;





/**
 * @brief Play a wave file currently must be S16LE format 2 channel stereo
 * @param filename  Filename to open, use getAbsolutePath
 * @param volume    Volume 0 - 128. SDL_MIX_MAXVOLUME constant for max volume
 * 
 */
extern void playSound(const char* file, int volume);
/**
 * @brief  Plays a new music, only 1 at a time plays
 *
 * @param filename  Filename of the WAVE file to load
 * @param volume  olume read playSound for more
 */
extern void playMusic(const char* file, int volume);
/**
 * @brief Plays a sound from a createAudio object (clones), only 1 at a time plays
 * Advantage to this method is no more disk reads, only once, data is stored and constantly reused
 * @param audio Audio object to clone and use
 * @param volume Volume read playSound for moree
 */
extern void playSoundFromMemory(Audio* audio, int volume);
/**
 * @brief Plays a music from a createAudio object (clones), only 1 at a time plays
 * Advantage to this method is no more disk reads, only once, data is stored and constantly reused
 *
 * @param audio  Audio object to clone and use
 * @param volume Volume read playSound for moree
 */
extern void playMusicFromMemory(Audio* audio, int volume);
/**
 * @brief Pause audio from playing
 */
extern void pauseAudio(void);
/**
 * @brief Unpause audio from playing
 */
extern void unPauseAudio(void);
/**
 * @brief ends an audio
 */
extern void endAudio(void);
/**
 * @brief Frees as many chained Audios as given
 *  @param audio Chain of sounds to free 
 */
void freeAudio(Audio * audio);
/**
 * @brief Initialize Audio Variable
 */
extern void initAudio(void);

/**
 * Create a Audio object
 *
 * @param file    Filename for the WAVE file to load
 * @param loop    0 ends after playing once (sound), 1 repeats and fades when other music added (music)
 * @param volume  Volume, read playSound()
 *
 * @return returns a new Audio or NULL on failure, you must call freeAudio() on return Audio
 *
 */
extern Audio* createAudio(const char* file, uint8_t loop, int volume);

#endif