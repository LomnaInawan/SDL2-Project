#include "core.h"
#include <stdlib.h>

#define WIDTH 1280
#define HEIGHT 720

typedef enum {playing, game_over} game_state;

game_state current_state = game_over;

const SDL_Color pipeColor = {0,180,0,255};
const SDL_Color birdColor = {255,255,0,255};

SDL_Rect bird = {580,0,50,50};
SDL_Rect pipes[6];

const int gravity = 2;
const int jump = 18;
const int gap = 200;
const int distancePipes = 350;
const int pipe_width = 150;

const int _pipes_speed = 7;
int _bird_velocity = 0;
const int _velocity_cap = 12;

void MouseButtonDown(Uint8 button){return;}
void MouseButtonUp(Uint8 button){return;}

int RandomPosition(){
  int r;
  r = rand() % (HEIGHT - (2 * gap));
  r += gap / 2;
  return r;
}

void OnStart(){
  srand(SDL_GetTicks());
  _bird_velocity = 0;
  bird.x = 580;
  bird.y = 0;
  SDL_Color background = {0,0,0,255};
  SetBackgroundColor(background);
  SetWindow(WIDTH, HEIGHT, 60, "Flappy Square");
  for(int i =0; i < 6; i++){
    int _pos = RandomPosition();
    pipes[i] = NewRectangle(WIDTH + (i * distancePipes),0,pipe_width,_pos);
    pipes[i + 1] = NewRectangle(WIDTH + (i * distancePipes), _pos + gap,pipe_width,HEIGHT);
    i++;
  }
}

void MovePipes(){
  for(int i = 0; i <6; i++){
    pipes[i].x -= _pipes_speed;
    if(pipes[i].x < -1 * pipe_width){
      pipes[i].x = WIDTH;
      pipes[i + 1].x = WIDTH;
      int _pos = RandomPosition();
      int _pos_x = (pipes[0].x > pipes[2].x) ? pipes[0].x : pipes[2].x;
      _pos_x = (_pos_x > pipes[4].x) ? _pos_x : pipes[4].x;
      _pos_x += distancePipes * 2;
      pipes[i] = NewRectangle(_pos_x,0,pipe_width,_pos);
      pipes[i + 1] = NewRectangle(_pos_x , _pos + gap,pipe_width,HEIGHT);
      i++;
      continue;
    }
    if(RectCollision(pipes[i], bird)) current_state = game_over;
  }
}

void DrawGame(){
  DrawSolidRectangle(bird, birdColor);
  for(int i =0; i < 6; i++)
    DrawSolidRectangle(pipes[i], pipeColor);
}

void PerFrameUpdate(){
  if(current_state == playing){
    _bird_velocity += (_bird_velocity >= _velocity_cap) ? 0 : gravity;
    if(KeyIsUp(SDLK_SPACE) && _bird_velocity >= 0)
      _bird_velocity = jump * -1;
    bird.y += _bird_velocity;
    MovePipes();
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
