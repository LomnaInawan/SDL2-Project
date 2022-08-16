#include "core.h"

const double pi = 355.0 / 113.0;

//General Maths functions
int absolute(int a){
    if (a >= 0) return a;
    else return -a;
}

int roundOff(double d){
    int a =  (int) d;
    if (d - a >= 0.5 && d >= 0) a++;
    else if (d - a >= 0.5 && d < 0) a--;
    return a;
}

double distBetweenPoints(SDL_Point a, SDL_Point b){
  double c = ((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y));
  c =  SDL_sqrt(c);
  return c;
}

SDL_Point addPoints(SDL_Point a, SDL_Point b){
    return newPoint(a.x + b.x, a.y + b.y);
}

vector2D addVectors(vector2D a, vector2D b){
    return newVector(a.x + b.x, a.y + b.y);
}

//This function returns angle in radians
double getVectorAngle(vector2D v){
    double value = v.x / SDL_sqrt(v.x * v.x + v.y * v.y);
    double angle = SDL_acos(value);
    if (v.y >= 0) return angle;
    else return (2 * pi - angle);
}

SDL_Point newPoint(int x, int y){
    SDL_Point t = {x,y};
    return t;
}

vector2D newVector(float x, float y){
    vector2D v = {x,y};
    return v;
}

SDL_Rect NewRectangle(int x, int y, int w, int h){
  SDL_Rect r = {x,y,w,h};
  return r;
}

//Check if two rectangles are colliding
bool RectCollision(SDL_Rect a, SDL_Rect b){
  //if(distBetweenPoints(a.midPoint, b.midPoint) > (a.radius + b.radius)) return false;
  int delta = a.y - b.y;
  int omega = b.x - a.x;
  return (delta <= b.h && delta >= -a.h && omega >= -b.w && omega <= a.w);
}

SDL_Point ContinuousCollision(SDL_Rect moving_rect, SDL_Point last_pos, SDL_Rect static_rect){
  SDL_Rect midRect = {0,0,moving_rect.w, moving_rect.h};
  SDL_Point endPoint = {moving_rect.x, moving_rect.y};
  while(absolute(moving_rect.x - last_pos.x) > moving_rect.w
        || absolute(moving_rect.y - last_pos.y) > moving_rect.h)
  {
    midRect.x = (last_pos.x + endPoint.x) / 2;
    midRect.y = (last_pos.y + endPoint.y) / 2;
    if(RectCollision(midRect, static_rect))
      endPoint = newPoint(midRect.x, midRect.y);
    else
      last_pos = newPoint(midRect.x, midRect.y);
  }
  return newPoint(midRect.x, midRect.y);
}
