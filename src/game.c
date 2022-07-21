#include "core.h"

SDL_Point mpos;

//1 is left click, 2 is middle click and 3 is right click
void MouseButtonDown(Uint8 button){
  printf("Button Down %d\n", button);
}

void MouseButtonUp(Uint8 button){
  printf("Button Up %d\n", button);
}

void OnStart(){
  SDL_Color background = {255,0,0,255};
  SetWindow(800, 600, 60, "Game");
  SetBackgroundColor(background);
}

int x, y;
void PerFrameUpdate(){
  if(KeyIsDown(SDLK_w)){
    printf("Key Down\n");
  }
  GetMousePosition(&mpos);
}
