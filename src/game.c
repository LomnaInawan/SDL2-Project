#include "core.h"

SDL_Point mpos;
renderObject *text;

//1 is left click, 2 is middle click and 3 is right click
void MouseButtonDown(Uint8 button){
  printf("Button Down %d\n", button);
}

void MouseButtonUp(Uint8 button){
  printf("Button Up %d\n", button);
}

void OnStart(){
  SDL_Color background = {255,255,255,255};
  SDL_Color textColor = {0, 0, 0, 255};

  ChangeCurrentFont("assets/font.ttf", 56);
  text = newRenderObject(NULL);
  text->texture = textureFromText("Hello World, This is a new line", textColor);
  Draw(text);
  SetWindow(800, 600, 60, "Game");
  SetBackgroundColor(background);
  PlayMusic("assets/music.wav");
  AudioDeviceIsPaused(false);
}

void PerFrameUpdate(){
  if(KeyIsUp(SDLK_w)){
    PlaySoundFX("assets/soundfx.wav");
    printf("Key Down\n");
  }
  GetMousePosition(&mpos);
}
