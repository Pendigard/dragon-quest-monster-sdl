#include "Sprite.h"

Sprite::Sprite()
{
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

Sprite &Sprite::operator=(Sprite const &other)
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

void Sprite::draw(int x, int y, Camera c, float size, int frame, bool bottomCoord, bool flip)
{
    destRect.w = width * size * c.zoom;
    destRect.h = height * size * c.zoom;
    destRect.x = (x - (destRect.w / 2) * bottomCoord) * c.zoom - c.x;
    destRect.y = (y - (destRect.h / 2) * bottomCoord) * c.zoom - c.y;
    int RenderCopy;
    int frameIndex = currentSprite;
    if (frame != -1)
        frameIndex = frame;
    if (!flip)
    {
        RenderCopy = SDL_RenderCopy(renderer, texture[frameIndex], &srcRect, &destRect);
    }
    else
    {
        RenderCopy = SDL_RenderCopyEx(renderer, texture[frameIndex], &srcRect, &destRect, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    if (RenderCopy != 0)
    {
        std::cout << path << " " << SDL_GetError() << std::endl;
    }
    assert(RenderCopy == 0);
}

void Sprite::draw(int x, int y, float size, int frame, bool bottomCoord, bool flip)
{
    destRect.w = width * size;
    destRect.h = height * size;
    destRect.x = x - (destRect.w / 2) * bottomCoord;
    destRect.y = y - (destRect.h / 2) * bottomCoord;
    int RenderCopy;
    int frameIndex = currentSprite;
    if (frame != -1)
        frameIndex = frame;
    if (!flip)
    {
        RenderCopy = SDL_RenderCopy(renderer, texture[frameIndex], &srcRect, &destRect);
    }
    else
        RenderCopy = SDL_RenderCopyEx(renderer, texture[frameIndex], &srcRect, &destRect, 0, NULL, SDL_FLIP_HORIZONTAL);
    if (RenderCopy != 0)
    {
        std::cout << path << " " << SDL_GetError() << std::endl;
    }
    assert(RenderCopy == 0);
}

void Sprite::drawCrop(int x, int y, int cropX, int cropY, Camera c, int cropW, int cropH, float size)
{
    destRect.w = cropW * size * c.zoom;
    destRect.h = cropH * size * c.zoom;
    destRect.x = (x * c.zoom - c.x);
    destRect.y = (y * c.zoom - c.y);
    srcRect.x = cropX;
    srcRect.y = cropY;
    srcRect.w = cropW;
    srcRect.h = cropH;
    int RenderCopy = SDL_RenderCopy(renderer, texture[currentSprite], &srcRect, &destRect);
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

void updateSprite(std::vector<Sprite> &vec)
{
    for (long unsigned int i = 0; i < vec.size(); i++)
    {
        vec[i].update();
    }
}

std::vector<SDL_Texture *> &Sprite::getTextures()
{
    return texture;
}

void loadMonsterSprite(SDL_Renderer *renderer, std::string type, std::unordered_map<std::string, Sprite> &sprites)
{
    if (sprites.find(type) != sprites.end()) {
        return;
    }
    std::string typeWithNoSpace = replaceString(type, " ", "_");
    sprites[type] = Sprite(renderer, "data/sprite/fighting_sprite/" + typeWithNoSpace + ".png", 1);
}

void loadOverworldSprite(SDL_Renderer *renderer, std::string name, std::unordered_map<std::string, Sprite> &sprites)
{
    if (name == "")
        return;
    std::string nameWithNoSpace = replaceString(name, " ", "_");
    sprites[name + "_front"] = Sprite(renderer, "data/sprite/overworld_sprites/" + nameWithNoSpace + "/" + nameWithNoSpace + "_front_", 3);
    sprites[name + "_back"] = Sprite(renderer, "data/sprite/overworld_sprites/" + nameWithNoSpace + "/" + nameWithNoSpace + "_back_", 3);
    sprites[name + "_left"] = Sprite(renderer, "data/sprite/overworld_sprites/" + nameWithNoSpace + "/" + nameWithNoSpace + "_left_", 3);
    sprites[name + "_right"] = Sprite(renderer, "data/sprite/overworld_sprites/" + nameWithNoSpace + "/" + nameWithNoSpace + "_right_", 3);
}

void loadSprite(SDL_Renderer *renderer, std::string name, std::string path, std::unordered_map<std::string, Sprite> &sprites, int numSprites)
{
    if (sprites.find(name) != sprites.end())
        return;
    sprites[name] = Sprite(renderer, path, numSprites);
}