#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "core.h"

#include <SDL2/SDL_ttf.h> //Remove if not using sdl_ttf

void StartAudioFunctions();
void AudioUpdate();
//Functions in text, remove if not using sdl_ttf
void StartTextFunctions(SDL_Renderer *_rend);
//Functions in draw.c file
void StartDrawFunctions(SDL_Renderer *renderer);
void Render();

int fps = 120;
int windowWidth = 1280;
int windowHeight = 720;
char *_windowTitle = "Title";
const Uint8* _keyboardState;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

SDL_Color _background = {0, 0, 0, 255};

Uint64 requiredDelta;
SDL_Event event;
int running = 1;

void GetMousePosition(SDL_Point* pos){
  SDL_GetMouseState(&(pos->x), &(pos->y));
}

bool KeyIsPressed(SDL_Scancode scode){
  _keyboardState = SDL_GetKeyboardState(NULL);
  if(_keyboardState[scode])
    return true;
  else
    return false;
}

bool KeyIsDown(SDL_KeyCode kcode){
  if (event.type == SDL_KEYDOWN && event.key.keysym.sym == kcode)
    {
      return true;
    }
  return false;
}

bool KeyIsUp(SDL_KeyCode kcode){
  if (event.type == SDL_KEYUP && event.key.keysym.sym == kcode)
    {
      return true;
    }
  return false;
}

void SetBackgroundColor(SDL_Color color){
  _background = color;
}

void SetWindow(int width, int height, int game_fps, char *title){
  windowHeight = height;
  windowWidth = width;
  fps = game_fps;
  _windowTitle = title;
}

void mainloop(){
  Uint64 startLoop = SDL_GetTicks();
  SDL_PumpEvents();
  SDL_PollEvent(&event);
  if(event.type == SDL_QUIT){
    running = 0;
  }else if(event.type == SDL_MOUSEBUTTONDOWN){
    MouseButtonDown(event.button.button);
  }else if(event.type == SDL_MOUSEBUTTONUP){
    MouseButtonUp(event.button.button);
  }
  SDL_SetRenderDrawColor(renderer, _background.r, _background.g, _background.b, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  PerFrameUpdate(); //The Update Function
  Render(); //Render the frame
  AudioUpdate(); //Update the audio queue
  SDL_RenderPresent(renderer);

  Uint64 delta = SDL_GetTicks() - startLoop;
  if (delta < requiredDelta)
  {
    SDL_Delay(requiredDelta - delta);
  }
}

int main(int argc, char *argv[]){
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
    printf("Error initializing SDL : %s",SDL_GetError());
    exit(EXIT_FAILURE);
  }

  window = SDL_CreateWindow("Title",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,windowWidth,windowHeight, SDL_WINDOW_RESIZABLE);
  if(window == NULL){
    printf("Failed to create window : %s",SDL_GetError());
    exit(EXIT_FAILURE);
  }

  TTF_Init(); //Remove if not using sdl_ttf

  renderer = SDL_CreateRenderer(window, -1 ,SDL_RENDERER_ACCELERATED);
  if(renderer == NULL){
    printf("Error creating renderer : %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  StartTextFunctions(renderer);
  StartDrawFunctions(renderer);
  StartAudioFunctions();
  OnStart();


  SDL_SetWindowSize(window, windowWidth, windowHeight);
  SDL_SetWindowTitle(window, _windowTitle);
  SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);
  requiredDelta = 1000/fps;

  #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainloop, 0, 1);
  #else
  while (running)
  {
    mainloop();
  }
  #endif

  TTF_Quit(); //Remove if not using sdl_ttf
  SDL_Quit();
  return EXIT_SUCCESS;
}
