#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "core.h"

int _objectsCount = 0;

//Local Data structures
typedef struct node{ 
    renderObject* obj;
    struct node* next; 
} node;

//Local Functions
void _removeNodeEntry(node *prev, node *current, node *next);
bool searchEntry(renderObject* object);
void appendEntry(renderObject* object);

SDL_Renderer *rend = NULL;

node *renderList;
node *_lastEntry;

//Add a new node at the end of the list
//Append a new entry in the renderList
void appendEntry(renderObject* object){
    node *new;
    new = (node*) malloc(sizeof(node));
    new->obj = object;
    new->next = NULL;
    _lastEntry->next = new;
    _lastEntry = new;
}

//Takes the three nodes from linked list and removes the middle(current) node
//This also frees the texture, hence needs to be loaded to draw again
void _removeNodeEntry(node *prev, node *current, node *next){
    if(current == renderList){
        node *a = renderList;
        SDL_DestroyTexture(renderList->obj->texture);
        renderList = renderList->next;
        free(a);
    }else{
        prev->next = next;
        SDL_DestroyTexture(current->obj->texture);
        free(current);
    }
}

//Remove entry by taking the pointer to render object 
void removeEntry(renderObject* object){
    if(searchEntry(object)){
        node *prev = NULL;
        node *last = renderList;
        while(true){
            if (last->obj == object)
            {
              _objectsCount--;
              printf("\nRemoved renderObject with ID : %p\n",(void*) object);
                _removeNodeEntry(prev, last, last->next);
                return;
            }
            prev = last;
            if(last->next == NULL){
                break;
            }
            last = last->next;
        }
    }else{ printf("\nEntry Does not Exitst : %p\n",(void*) object); }
}

//Search entry in the renderList
bool searchEntry(renderObject* object){
    node *last = renderList;
    while(true){
        if(last->obj == object){
            return true;
        }
        if(last->next == NULL){
            break;
        }
        last = last->next;
    }
    return false;
}

SDL_Texture* textureFromImage(char *img_path){
  SDL_Surface *surface;
  SDL_Texture *texture;
  surface = SDL_LoadBMP(img_path);
  texture = SDL_CreateTextureFromSurface(rend, surface);
  SDL_FreeSurface(surface);
  return texture;
}

//Initializes a render object by taking the path to an image
renderObject* newRenderObject(char *img_path){
  renderObject* r = (renderObject*) malloc(sizeof(renderObject));

  if(img_path != NULL){
    SDL_Surface *surface;
    SDL_Texture *texture;
    surface = SDL_LoadBMP(img_path);
    texture = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    r->texture = texture;
  }else{
    r->texture = NULL;
  }
  SDL_RendererFlip f = SDL_FLIP_NONE;
  SDL_Point position = {0,0};
  vector2D scale = {1,1};
  double a = 0;

  r->position = position;
  r->scale = scale;
  r->anchor = position;
  r->angle = a;
  r->flip = f;
  return r;
}

//Initialize the variables required by the script
void StartDrawFunctions(SDL_Renderer *renderer){
    rend = renderer;
    renderList = (node*) malloc(sizeof(node));
    renderList->next = NULL;
    renderList->obj = NULL;
}


//Append an entry in the renderlist
void Draw(renderObject* object){
    if(_objectsCount == 0){
        _objectsCount++;
        renderList->obj = object;
        _lastEntry = renderList;
        return;
    }
    if (searchEntry(object))
    {
        return;
    }else{
        appendEntry(object);
        _objectsCount++;
    }
}

//The function is called at the end of each frame to copy everything from renderframe to renderer
void Render(){
    if(_objectsCount == 0)
      return;

    node* last = renderList;
    while (true)
    {
        vector2D v = last->obj->scale;
        int tw, th;
        SDL_QueryTexture(last->obj->texture,NULL,NULL,&tw,&th);
        tw = roundOff(tw * v.x);
        th = roundOff(th * v.y);
        SDL_Rect r = {last->obj->position.x, last->obj->position.y,tw, th};
        SDL_RenderCopyEx(rend,last->obj->texture,NULL,&r,last->obj->angle,&(last->obj->anchor),last->obj->flip);
        if(last->next == NULL){
            break;
        }
        last = last->next;
    }
}
