#include "Map.h"

Map::Map()
{
    this->width = 0;
    this->height = 0;
    this->map = std::vector<std::vector<int> >();
}

void Map::loadMap(std::string path, Database &db)
{
    rapidjson::Document doc;
    loadJson(path, doc);
    this->width = doc["width"].GetInt();
    this->height = doc["height"].GetInt();
    for (size_t i=0;i<doc["properties"].Size();i++) {
        if (doc["properties"][i]["name"].GetString() == (std::string) "levelMin") {
            this->levelMin = doc["properties"][i]["value"].GetInt();
        }
        if (doc["properties"][i]["name"].GetString() == (std::string) "levelMax") {
            this->levelMax = doc["properties"][i]["value"].GetInt();
        }
        if (doc["properties"][i]["name"].GetString() == (std::string) "monsters") {
            this->monsters = splitString(doc["properties"][i]["value"].GetString(), ";");
        }
    }
    for (size_t i = 0; i < doc["layers"].Size(); i++)
    {
        if (doc["layers"][i]["type"].GetString() == (std::string) "tilelayer")
        {
            std::vector<int> layer;
            for (size_t j = 0; j < doc["layers"][i]["data"].Size(); j++)
            {
                layer.push_back(doc["layers"][i]["data"][j].GetInt());
            }
            this->map.push_back(layer);
        }
        if (doc["layers"][i]["type"].GetString() == (std::string) "objectgroup")
        {
            if (doc["layers"][i]["name"].GetString() == (std::string) "hitbox")
            {
                this->hitboxes.resize(doc["layers"][i]["objects"].Size());
                for (size_t j = 0; j < doc["layers"][i]["objects"].Size(); j++)
                {
                    float w = doc["layers"][i]["objects"][j]["width"].GetFloat();
                    float h = doc["layers"][i]["objects"][j]["height"].GetFloat();
                    float x = doc["layers"][i]["objects"][j]["x"].GetFloat();
                    float y = doc["layers"][i]["objects"][j]["y"].GetFloat();
                    hitboxes[j] = Hitbox(x, y, w, h);
                }
            }
            if (doc["layers"][i]["name"].GetString() == (std::string) "script")
            {
                for (size_t j = 0; j < doc["layers"][i]["objects"].Size(); j++)
                {
                    if (doc["layers"][i]["objects"][j]["name"].GetString() == (std::string)"respawn") {
                        respawnX = doc["layers"][i]["objects"][j]["x"].GetFloat();
                        respawnY = doc["layers"][i]["objects"][j]["y"].GetFloat();
                    }
                }
            }
            if (doc["layers"][i]["name"].GetString() == (std::string) "entity")
            {
                this->entities.resize(doc["layers"][i]["objects"].Size());
                for (size_t j = 0; j < doc["layers"][i]["objects"].Size(); j++)
                {
                    float x = doc["layers"][i]["objects"][j]["x"].GetFloat();
                    float y = doc["layers"][i]["objects"][j]["y"].GetFloat();
                    std::string name = doc["layers"][i]["objects"][j]["name"].GetString();
                    std::string type = doc["layers"][i]["objects"][j]["type"].GetString();
                    entityTypes entType = NPC;
                    if (type == "monster")
                    {
                        entType = MONSTER;
                    }
                    std::string ai;
                    std::string script = "";
                    Direction d = MOVE_DOWN;
                    int offsetX, offsetY = 0;
                    for (size_t k = 0; k < doc["layers"][i]["objects"][j]["properties"].Size(); k++)
                    {
                        if (doc["layers"][i]["objects"][j]["properties"][k]["name"].GetString() == (std::string) "ai_type")
                        {
                            ai = doc["layers"][i]["objects"][j]["properties"][k]["value"].GetString();
                        }
                        if (doc["layers"][i]["objects"][j]["properties"][k]["name"].GetString() == (std::string) "script")
                        {
                            script = doc["layers"][i]["objects"][j]["properties"][k]["value"].GetString();
                        }
                        if (doc["layers"][i]["objects"][j]["properties"][k]["name"].GetString() == (std::string) "offset_x")
                        {
                            offsetX = doc["layers"][i]["objects"][j]["properties"][k]["value"].GetInt();
                        }
                        if (doc["layers"][i]["objects"][j]["properties"][k]["name"].GetString() == (std::string) "offset_y")
                        {
                            offsetY = doc["layers"][i]["objects"][j]["properties"][k]["value"].GetInt();
                        }
                        if (doc["layers"][i]["objects"][j]["properties"][k]["name"].GetString() == (std::string) "direction")
                        {
                            std::string direction = doc["layers"][i]["objects"][j]["properties"][k]["value"].GetString();
                            if (direction == "up")
                                d = MOVE_UP;
                            else if (direction == "down")
                                d = MOVE_DOWN;
                            else if (direction == "left")
                                d = MOVE_LEFT;
                            else if (direction == "right")
                                d = MOVE_RIGHT;
                        }
                    }
                    aiType aiT = AI_NONE;
                    if (ai == "random")
                    {
                        aiT = AI_RANDOM;
                    }
                    else if (ai == "left_right")
                        aiT = AI_LEFT_RIGHT;
                    if (entType == MONSTER)
                        this->entities[j] = Entity(name, x, y, db, aiT);
                    else
                    {
                        float w = doc["layers"][i]["objects"][j]["width"].GetFloat();
                        float h = doc["layers"][i]["objects"][j]["height"].GetFloat();
                        this->entities[j] = Entity(name, x, y, w, h, aiT, offsetX, offsetY,d);
                        if (script != "")
                        {
                            this->entities[j].setScript(script);
                        }
                    }
                }
            }
        }
    }
    for (size_t i = 0; i < doc["tilesets"].Size(); i++)
    {
        this->firstGids.push_back(doc["tilesets"][i]["firstgid"].GetInt());
        this->tileSets[doc["tilesets"][i]["firstgid"].GetInt()] = doc["tilesets"][i]["source"].GetString();
    }
}

int Map::getValue(int x, int y, int layer) const
{
    assert(x >= 0 && x < this->width);
    assert(y >= 0 && y < this->height);
    return this->map[layer][y * this->width + x];
}

int Map::getWidth() const
{
    return this->width;
}

int Map::getHeight() const
{
    return this->height;
}

std::string Map::getTileSet(int gid) const
{
    assert(gid > 0);
    return this->tileSets.at(gid);
}

int Map::getFirstGid(int value) const
{
    if (value <= 0)
    {
        return 0;
    }
    for (size_t i = 0; i < this->firstGids.size(); i++)
    {
        if (this->firstGids[i] > value)
        {
            return this->firstGids[i - 1];
        }
    }
    return this->firstGids[this->firstGids.size() - 1];
}

int Map::getLayerCount() const
{
    return this->map.size();
}

void Map::drawLayer(int layer, SDL_Renderer *renderer, Camera camera, std::unordered_map<std::string, Sprite> &sprites, Database &db) const
{
    for (size_t i = 0; i < this->map[layer].size(); i++)
    {
        int x = i % this->width;
        int y = i / this->width;
        int value = this->map[layer][i];
        if (value > 0 && tileInCamera(camera, x * 32, y * 32))
        {
            int firstGid = this->getFirstGid(value);
            std::string tileSet = this->getTileSet(firstGid);
            int tileId = value - firstGid;
            int tileWidth = db.getTileWidth(tileSet);
            int tileHeight = db.getTileHeight(tileSet);
            int tileX = tileId % (db.getTileSetWidth(tileSet) / tileWidth);
            int tileY = tileId / (db.getTileSetWidth(tileSet) / tileWidth);
            sprites[db.getImage(tileSet)].drawCrop(x * tileWidth, y * tileHeight, tileX * tileWidth, tileY * tileHeight, camera);
        }
    }
}

std::vector<Hitbox> Map::getHitboxes() const
{
    return hitboxes;
}

std::vector<Entity> Map::getEntities() const
{
    return entities;
}

bool tileInCamera(Camera &c, int x, int y)
{
    int tile_width = 32;  // Largeur d'une case
    int tile_height = 32; // Hauteur d'une case

    // Convertir les coordonnées de la case en pixels
    int tile_x = x * c.zoom;
    int tile_y = y * c.zoom;

    // Vérifier si la case est dans la caméra
    if (tile_x + tile_width * c.zoom >= c.x &&
        tile_x <= c.x + c.w &&
        tile_y + tile_height * c.zoom >= c.y &&
        tile_y <= c.y + c.h)
    {
        return true;
    }

    return false;
}

std::vector<std::string> Map::getMonsters(unsigned int &levelMin, unsigned int &levelMax) const {
    levelMin = this->levelMin;
    levelMax = this->levelMax;
    return this->monsters;
}

void Map::getRespawnCoord(unsigned int &x, unsigned int &y) const {
    x = this->respawnX;
    y = this->respawnY;
}
