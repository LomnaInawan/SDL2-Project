#pragma once
#include <SDL2/SDL.h>

typedef enum {false, true} bool;

typedef struct
{
    float x;
    float y;
} vector2D;

typedef struct 
{
    SDL_Texture *texture;
    SDL_Point position;
    vector2D scale;
    SDL_Point anchor;
    double angle;
    SDL_RendererFlip flip;
} renderObject;

//Functions in draw file
renderObject* newRenderObject(char *img_path);
void Draw(renderObject* object);
void removeEntry(renderObject* object);
SDL_Texture* textureFromImage(char *img_path);

//Input functions in core.c file
bool KeyIsDown(SDL_KeyCode kcode);
bool KeyIsUp(SDL_KeyCode kcode);
bool KeyIsPressed(SDL_Scancode scode);
void GetMousePosition(SDL_Point* pos);

//Function in basic collision
int absolute(int a);
int roundOff(double d);
double distBetweenPoints(SDL_Point a, SDL_Point b);
SDL_Point addPoints(SDL_Point a, SDL_Point b);
vector2D addVectors(vector2D a, vector2D b);
double getVectorAngle(vector2D v);
SDL_Point newPoint(int x, int y);
vector2D newVector(float x, float y);

//Functions in text
int ChangeCurrentFont(char *fontPath, int fontSize);
SDL_Texture* textureFromText(char *text, SDL_Color textColor);

//Functions in audio
void PlayMusic(char *filePath);
void AudioDeviceIsPaused(bool pause);
void PlaySoundFX(char *filePath);

//Functions used in game file
void PerFrameUpdate();
void OnStart();
void SetBackgroundColor(SDL_Color color);
void SetWindow(int width, int height, int game_fps, char *title);
void MouseButtonDown(Uint8 button);
void MouseButtonUp(Uint8 button);
