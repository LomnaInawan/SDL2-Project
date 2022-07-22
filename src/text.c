#include <SDL2/SDL_ttf.h>
#include "core.h"

SDL_Renderer *_text_rend;
TTF_Font *current_font = NULL;

void StartTextFunctions(SDL_Renderer *_rend){
  _text_rend = _rend;
}

int ChangeCurrentFont(char *fontPath, int fontSize){
  current_font = TTF_OpenFont(fontPath, fontSize);
  if(current_font == NULL){
    printf("Loading Font Failed ");
    return 1;
  }else{
    return 0;
  }
}

SDL_Texture* textureFromText(char *text, SDL_Color textColor){
  SDL_Surface *surface;
  SDL_Texture *texture;

  surface = TTF_RenderText_Solid(current_font, text, textColor);
  texture = SDL_CreateTextureFromSurface(_text_rend, surface);
  SDL_FreeSurface(surface);
  return texture;
}
