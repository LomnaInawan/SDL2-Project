#include "core.h"
#include <stdlib.h>

#define WIDTH 1280
#define HEIGHT 720

typedef enum {playing, game_over} game_state;

game_state current_state = playing;
const SDL_Color birdColor = {255,255,0,255};
SDL_Rect bird = {580,0,50,50};

const SDL_Color platformColor = {0,255,0,255};
SDL_Rect platform = {690, 400, 100, 100};

const int gravity = 2;
const int jump = 18;
const int _velocity_cap = 12;
const int velocityX = 10;

int _bird_velocityY = 0;
int _bird_velocityX = 0;

void MouseButtonDown(Uint8 button){return;}
void MouseButtonUp(Uint8 button){return;}

void OnStart(){
  _bird_velocityY = 0;
  bird.x = 580;
  bird.y = 0;
  SDL_Color background = {5,5,5,255};
  SetBackgroundColor(background);
  SetWindow(WIDTH, HEIGHT, 60, "Flappy Square");
}

void DrawGame(){
  DrawSolidRectangle(bird, birdColor);
  DrawSolidRectangle(platform, platformColor);
}

SDL_Point _last_pos;
void PerFrameUpdate(){
  if(current_state == playing){
    //Movement in Y axis
    _bird_velocityY = ( KeyIsPressed(SDL_SCANCODE_S) - KeyIsPressed(SDL_SCANCODE_W) ) * velocityX;
    //Movement in X axis
    _bird_velocityX = ( KeyIsPressed(SDL_SCANCODE_D) - KeyIsPressed(SDL_SCANCODE_A) ) * velocityX;

    //Set position according to velocity
    _last_pos = newPoint(bird.x, bird.y);
    bird.y += _bird_velocityY;
    bird.x += _bird_velocityX;

    if(RectCollision(bird, platform)){
      SDL_Point tempPoint = ContinuousCollision(bird, _last_pos, platform, 4);
      bird.x = tempPoint.x;
      bird.y = tempPoint.y;
    }

    //Offscreen gameover
    if(bird.y > (HEIGHT - 30) || bird.y < -30)
      current_state = game_over;
  }else{
    if(KeyIsUp(SDLK_SPACE)){
      current_state = playing;
      OnStart();
    }
  }
  DrawGame();
}
