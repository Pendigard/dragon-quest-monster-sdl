#include "Sprite.h"

Sprite::Sprite() {
    this->currentSprite = 0;
    this->isAnimated = false;
    this->path = "";
    this->renderer = nullptr;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = 0;
    srcRect.h = 0;
}

Sprite::Sprite(SDL_Renderer *renderer, std::string path, int numSprites)
{
    this->currentSprite = 0;
    this->isAnimated = false;
    this->path = path;
    this->renderer = renderer;
    srcRect.x = 0;
    srcRect.y = 0;
    loadTexture(path, numSprites);
    srcRect.w = width;
    srcRect.h = height;
}

void Sprite::loadTexture(std::string path, int numSprites)
{
    texture.clear();
    if (numSprites == 1)
    {
        SDL_Surface *surface = IMG_Load(path.c_str());
        width = surface->w;
        height = surface->h;
        SDL_Texture *temp = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        texture.push_back(temp);
    }
    else
    {
        this->isAnimated = true;
        for (int i = 0; i < numSprites; i++)
        {
            std::string tmpPath = path + std::to_string(i) + ".png";
            SDL_Surface *surface = IMG_Load(tmpPath.c_str());
            width = surface->w;
            height = surface->h;
            SDL_Texture *temp = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            texture.push_back(temp);
        }
    }
}

Sprite::Sprite(Sprite const &other)
{
    this->width = other.getWidth();
    this->height = other.getHeight();
    this->currentSprite = 0;
    this->isAnimated = other.getIsAnimated();
    this->renderer = other.getRenderer();
    this->path = other.getPath();
    srcRect.x = 0;
    srcRect.y = 0;
    loadTexture(path, other.getNumSprites());
    srcRect.w = width;
    srcRect.h = height;
}

Sprite& Sprite::operator=(Sprite const &other)
{
    this->width = other.getWidth();
    this->height = other.getHeight();
    this->currentSprite = 0;
    this->isAnimated = other.getIsAnimated();
    this->renderer = other.getRenderer();
    this->path = other.getPath();
    srcRect.x = 0;
    srcRect.y = 0;
    loadTexture(path, other.getNumSprites());
    srcRect.w = width;
    srcRect.h = height;
    return *this;
}

Sprite::~Sprite()
{
    for (int i = 0; i < texture.size(); i++)
    {
        SDL_DestroyTexture(texture[i]);
    }
    texture.clear();
}

void Sprite::draw(int x, int y, Camera c, bool notAbsolute, float size, bool bottomCoord, bool flip)
{
    destRect.w = width * size;
    destRect.h = height * size;
    destRect.x = (x - (destRect.w / 2) * bottomCoord) - c.x * notAbsolute;
    destRect.y = y - (destRect.h / 2) * bottomCoord - c.y * notAbsolute;
    int RenderCopy;
    if (!flip)
    {
        RenderCopy = SDL_RenderCopy(renderer, texture[currentSprite], &srcRect, &destRect);
    }
    else
        RenderCopy = SDL_RenderCopyEx(renderer, texture[currentSprite], &srcRect, &destRect, 0, NULL, SDL_FLIP_HORIZONTAL);
    assert(RenderCopy == 0);
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
    currentSprite = 0;
}

bool Sprite::getIsAnimated() const
{
    return isAnimated;
}

int Sprite::getWidth() const
{
    return width;
}

int Sprite::getHeight() const
{
    return height;
}

std::string Sprite::getPath() const
{
    return path;
}

SDL_Renderer *Sprite::getRenderer() const
{
    return renderer;
}

unsigned int Sprite::getNumSprites() const
{
    return texture.size();
}

void Sprite::setCurrentSprite(int currentSprite)
{
    this->currentSprite = currentSprite;
}

void drawText(SDL_Renderer *renderer, std::string text, int x, int y, int size, SDL_Color color)
{
    TTF_Font *font = TTF_OpenFont("data/font/a-goblin-appears.ttf", size);
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void drawBox(SDL_Renderer *renderer, int x, int y, int w, int h)
{
    SDL_Rect rectFill = {x + 5,y + 5, w - 10,h - 10};
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 170);
    SDL_RenderFillRect(renderer, &rectFill);
}

void updateSprite(std::vector<Sprite> &vec)
{
    for (long unsigned int i = 0; i < vec.size(); i++)
    {
        vec[i].update();
    }
}