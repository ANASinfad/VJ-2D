#pragma once
#include "Sprite.h"
#include "TileMap.h"
class WaterDrop
{
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    void setPlayerDown();

    glm::ivec2 getPosition();

    glm::ivec2 getEnemySize();

    pair<glm::ivec2, glm::ivec2> getBoundingBox();

    bool playerIsDmaged();

    void setDamagedPlayer();


private:
    glm::ivec2 tileMapDispl, posWater, waterSize, posInit;
    bool renderTile, crashed, damagedPlayer;
    int state;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
    float delayTime;
};

