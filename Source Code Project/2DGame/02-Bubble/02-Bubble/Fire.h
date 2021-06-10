#pragma once
#include "Sprite.h"
#include "TileMap.h"
class Fire
{
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);

    glm::ivec2 getPosition();

    glm::ivec2 getEnemySize();

    pair<glm::ivec2, glm::ivec2> getBoundingBox();

    bool playerIsDmaged();
    bool getVisible();
    void setPlayerNear();
    glm::ivec2 getBase();
    bool isNear();
    int getState();
    void setDamagedPlayer();
private:
    glm::ivec2 tileMapDispl, posFire, fireSize;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
    int state;
    bool damagedPlayer, playerNear, visible;
    float delayTime;
};

