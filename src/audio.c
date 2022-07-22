#include "core.h"
#include <stdio.h>
#include <stdlib.h>

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

void PlaySoundFX(char *filePath, int volume){
  Uint8* soundfx_buffer;
  Uint32 soundfx_lenght;

  Uint32 size = SDL_GetQueuedAudioSize(audio_device);
  SDL_ClearQueuedAudio(audio_device);
  Uint8* ptr = wav_buffer + wav_lenght - size;
  SDL_LoadWAV(filePath, NULL, &soundfx_buffer, &soundfx_lenght);
  SDL_MixAudioFormat(ptr, soundfx_buffer, FORMAT, soundfx_lenght, volume);
  SDL_QueueAudio(audio_device, ptr, size);
}

void PlayMusic(char *filePath){
  SDL_ClearQueuedAudio(audio_device);
  if(wav_buffer != NULL){
    SDL_FreeWAV(wav_buffer);
  }
  SDL_AudioSpec wav_spec;

  if(SDL_LoadWAV(filePath, &wav_spec, &wav_buffer, &wav_lenght) == NULL){
    printf("\nFailed to load WAV : %s\n", filePath);
  }else{
    SDL_QueueAudio(audio_device, wav_buffer, wav_lenght);
    //SDL_QueueAudio(audio_device, wav_buffer, wav_lenght);
  }
}

int _count = 0;
void AudioUpdate(){
  if(SDL_GetQueuedAudioSize(audio_device) <= wav_lenght){
    SDL_QueueAudio(audio_device, wav_buffer, wav_lenght);
  }
  if(_count == 0){
    //printf("%d\n", SDL_GetQueuedAudioSize(audio_device));  
    _count = 60;
  }
  _count--;
}
