#include <stdio.h>
#include <stdlib.h>
#include "core.h"

#define FREQUENCY 48000
#define FORMAT AUDIO_S16LSB
#define CHANNELS 2
#define SAMPLES 4096
#define MAX_SOUND 25
#define FADE_VALUE 2
#define CHANGES SDL_AUDIO_ALLOW_ANY_CHANGE

SDL_AudioDeviceID audio_device;
SDL_AudioSpec desired, obtained;

Uint8* wav_buffer = NULL;
Uint32 wav_lenght;
SDL_AudioSpec wav_spec;

void StartAudioFunctions(){
  SDL_zero(desired);
  desired.freq = FREQUENCY;
  desired.format = FORMAT;
  desired.channels= CHANNELS;
  desired.samples = SAMPLES;
  desired.callback = NULL;
  audio_device = SDL_OpenAudioDevice(NULL, 0, &desired, &obtained, CHANGES);
  if(audio_device == 0)
    printf("\nFailed to open audio device : %s\n", SDL_GetError());
  if(desired.format != obtained.format)
    printf("\nDid not get the desired audio format\n");
}

void AudioDeviceIsPaused(bool pause){
  SDL_PauseAudioDevice(audio_device, pause);
}

void PlaySoundFX(char *filePath){
  Uint8* load_buf;
  Uint32 load_len;
  SDL_AudioSpec load_spec;

  //Load soundfx buffer
  SDL_LoadWAV(filePath, &load_spec, &load_buf, &load_len);

  if(wav_buffer == NULL){
    //No background music
    SDL_LoadWAV(filePath, &load_spec, &load_buf, &load_len);
    SDL_ClearQueuedAudio(audio_device);
    SDL_QueueAudio(audio_device, load_buf, load_len);
  }else{
    AudioDeviceIsPaused(true);
    //The modified buffer with soundfx and background music mixed
    static void* soundfx_buffer = NULL;
    if(soundfx_buffer != NULL)
      free(soundfx_buffer);
    Uint32 soundfx_lenght = SDL_GetQueuedAudioSize(audio_device);
    soundfx_buffer = (void*) malloc(soundfx_lenght);
    //Copy the current audio buffer to be modified
    memcpy(soundfx_buffer,(void*) (wav_buffer - soundfx_lenght + (2 * wav_lenght)), wav_lenght - ((2 * wav_lenght) - soundfx_lenght));
    memcpy(soundfx_buffer + (wav_lenght - ((2 * wav_lenght) - soundfx_lenght)),(void*) wav_buffer, wav_lenght);
    //Mix buffers
    SDL_MixAudioFormat( soundfx_buffer, load_buf, wav_spec.format,load_len, SDL_MIX_MAXVOLUME);
    //Load mixed audio buffer
    SDL_ClearQueuedAudio(audio_device);
    SDL_QueueAudio(audio_device, soundfx_buffer, soundfx_lenght);
    AudioDeviceIsPaused(false);
  }
  //Free loaded soundfx buffer
  SDL_FreeWAV(load_buf);
}

void PlayMusic(char *filePath){
  SDL_ClearQueuedAudio(audio_device);
  if(wav_buffer != NULL){
    SDL_FreeWAV(wav_buffer);
  }

  if(SDL_LoadWAV(filePath, &wav_spec, &wav_buffer, &wav_lenght) == NULL){
    printf("\nFailed to load WAV : %s\n", filePath);
  }else{
    SDL_QueueAudio(audio_device, wav_buffer, wav_lenght);
    //SDL_QueueAudio(audio_device, wav_buffer, wav_lenght);
  }
}

void FreeMusic(){
  if(wav_buffer != NULL)
    SDL_FreeWAV(wav_buffer);
}

void AudioUpdate(){
  if(SDL_GetQueuedAudioSize(audio_device) <= wav_lenght){
    SDL_QueueAudio(audio_device, wav_buffer, wav_lenght);
  }
}
