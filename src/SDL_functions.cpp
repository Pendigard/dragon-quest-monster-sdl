
#include "SDL_functions.h"

void drawText(SDL_Renderer *renderer, std::string text, int x, int y, int size, SDL_Color color)
{
    TTF_Font *font = TTF_OpenFont("data/font/a-goblin-appears.ttf", size);
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
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
    SDL_Rect rectFill = {x + 5, y + 5, w - 10, h - 10};
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 170);
    SDL_RenderFillRect(renderer, &rectFill);
}

void drawDialogBox(SDL_Renderer *renderer, std::string text, std::string name,const unsigned int size)
{
    SDL_Color color = {255, 255, 255};
    if (name != "") {
        drawBox(renderer, 15, 520, name.size()*17, 30);
        drawText(renderer, name, 25, 530, 12, color);
    }
    std::vector<std::string> dialog = splitString(text, "|");
    drawBox(renderer, 15, 550, 770, 100);
    for (size_t i = 0; i < dialog.size(); i++)
    {
        drawText(renderer, dialog[i], 25, 560 + i * 20, size, color);
    }
}

void playMusic(Mix_Music* music)
{
    if (Mix_PlayingMusic() == 0)
    {
        Mix_PlayMusic(music, -1);
    }
    else
    {
        Mix_HaltMusic();
        Mix_PlayMusic(music, -1);
    }
}

void playSound(Mix_Chunk* sound)
{
    Mix_PlayChannel(-1, sound, 0);
}