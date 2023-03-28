
#include "Menu.h"

Option createOption(std::string text, int x, int y, int w, int h, cursorDirection direction, menuChoice action, bool transparent)
{
    Option option;
    option.text = text;
    option.x = x;
    option.y = y;
    option.w = w;
    option.h = h;
    option.direction = direction;
    option.transparent = transparent;
    option.action = action;
    return option;
}

Menu::Menu()
{
    this->currentChoiceX = 0;
    this->currentChoiceY = 0;
    this->cursor = NULL;
}

Menu::Menu(Menu const &other)
{
    this->currentChoiceX = other.currentChoiceX;
    this->currentChoiceY = other.currentChoiceY;
    this->cursor = other.cursor;
    this->options = other.options;
}

Menu::Menu(Sprite *cursor)
{
    this->currentChoiceX = 0;
    this->currentChoiceY = 0;
    this->cursor = cursor;
    this->currentPage = 0;
    this->changePageX = true;
}

void Menu::addRow(std::vector<Option> row, int page)
{
    if (page >= options.size())
    {
        options.resize(page + 1);
    }
    options[page].push_back(row);
}

void Menu::drawOptions(SDL_Renderer *renderer, Camera camera)
{
    for (int i = 0; i < options[currentPage].size(); i++)
    {
        for (int j = 0; j < options[currentPage][i].size(); j++)
        {
            if (!options[currentPage][i][j].transparent)
            {
                drawBox(renderer, options[currentPage][i][j].x, options[currentPage][i][j].y, options[currentPage][i][j].w, options[currentPage][i][j].h);
                SDL_Color color = {255, 255, 255};
                drawText(renderer, options[currentPage][i][j].text, options[currentPage][i][j].x + 10, options[currentPage][i][j].y + 10, 20, color);
            }
            if (i == currentChoiceY && j == currentChoiceX)
            {
                (*cursor).setCurrentSprite(options[currentPage][i][j].direction);
                switch (options[currentPage][i][j].direction)
                {
                case UP:
                    (*cursor).draw(options[currentPage][i][j].x + options[currentPage][i][j].w / 2 - (*cursor).getWidth() / 2, options[currentPage][i][j].y - (*cursor).getHeight(), camera, false, 1);
                    break;
                case DOWN:
                    (*cursor).draw(options[currentPage][i][j].x + options[currentPage][i][j].w / 2 - (*cursor).getWidth() / 2, options[currentPage][i][j].y + options[currentPage][i][j].h, camera, false, 1);
                    break;
                case LEFT:
                    (*cursor).draw(options[currentPage][i][j].x - (*cursor).getWidth(), options[currentPage][i][j].y + options[currentPage][i][j].h / 2 - (*cursor).getHeight() / 2, camera, false, 1);
                    break;
                case RIGHT:
                    (*cursor).draw(options[currentPage][i][j].x + options[currentPage][i][j].w, options[currentPage][i][j].y + options[currentPage][i][j].h / 2 - (*cursor).getHeight() / 2, camera, false, 1);
                    break;
                }
            }
        }
    }
}

int Menu::getCurrentChoiceX() const
{
    return currentChoiceX;
}

int Menu::getCurrentChoiceY() const
{
    return currentChoiceY;
}

int Menu::getCurrentPage() const
{
    return currentPage;
}

void Menu::changeCurrentChoice(int x, int y)
{
    currentChoiceX = x;
    if (currentChoiceX < 0)
    {
        if (changePageX)
            currentPage = std::max(currentPage - 1, 0);
        currentChoiceX = options[currentPage][currentChoiceY].size() - 1;
    }
    if (currentChoiceX >= options[currentPage][currentChoiceY].size())
    {
        if (changePageX)
            currentPage = (currentPage + 1) % options.size();;
        currentChoiceX = 0;
    }
    currentChoiceY = y;
    if (currentChoiceY < 0)
    {
        if (!changePageX)
            currentPage = std::max(currentPage - 1, 0);
        currentChoiceY = options[currentPage].size() - 1;
    }
    if (currentChoiceY >= options[currentPage].size())
    {
        if (!changePageX)
            currentPage = (currentPage + 1) % options.size();
        currentChoiceY = 0;
    }
}

menuChoice Menu::getChoice() const
{
    return options[currentPage][currentChoiceY][currentChoiceX].action;
}

void Menu::clear()
{
    options.clear();
}