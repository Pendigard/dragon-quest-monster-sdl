#ifndef _SPRITE
#define _SPRITE

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
#include <vector>


class Sprite
{
private:
    std::vector<SDL_Texture*> texture;
    SDL_Rect srcRect, destRect;
    int width, height;
    bool isAnimated;

public:
    Sprite(SDL_Renderer* renderer, std::string path, int w, int h, int numSprites);
    ~Sprite();
    void draw(SDL_Renderer* renderer,int x,int y);
    void update();
    void changeAnimation();
    bool getIsAnimated() const;

};

#endif