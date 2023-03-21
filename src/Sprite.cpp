#include "Sprite.h"

Sprite::Sprite(SDL_Renderer *renderer, std::string path, int x, int y, int w, int h, int numSprites)
{
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;
    this->currentSprite = 0;
    for (int i = 0; i < numSprites; i++)
    {
        std::string tmpPath = path + std::to_string(i) + ".png";
        SDL_Surface *surface = IMG_Load(tmpPath.c_str());
        SDL_Texture *temp = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        texture.push_back(temp);
    }
}

Sprite::~Sprite()
{
    for (int i = 0; i < texture.size(); i++)
    {
        SDL_DestroyTexture(texture[i]);
    }
}

void Sprite::draw(SDL_Renderer *renderer,int x,int y)
{
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = width;
    srcRect.h = height;

    destRect.x = x;
    destRect.y = y;
    destRect.w = width;
    destRect.h = height;

    SDL_RenderCopy(renderer, texture[currentSprite], &srcRect, &destRect);
}

void Sprite::update()
{
    currentSprite++;
    if (currentSprite >= texture.size())
    {
        currentSprite = 0;
    }
}

void Sprite::changeAnimation()
{
    isAnimated = !isAnimated;
}

bool Sprite::getIsAnimated() const
{
    return isAnimated;
}