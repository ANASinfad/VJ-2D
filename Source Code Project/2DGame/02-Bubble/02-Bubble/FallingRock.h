#pragma once
#include "Sprite.h"
#include "TileMap.h"

class FallingRock
{
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    bool update(int deltaTime, glm::ivec2 playerPos);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    void setPlayerDown();

    glm::ivec2 getPosition();

    glm::ivec2 getEnemySize();

    bool playerIsDmaged();

    void setDamagedPlayer();

    void setCrashed();

    bool getCrashed();

    pair <glm::ivec2, glm::ivec2> getBoundingBox();

    void checkIfPlayerIsUnderRock(glm::ivec2 posPlayer);



private:
    glm::ivec2 tileMapDispl, posRock, rockSize;
    bool playerDown, crashed, damagedPlayer;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
    float delayTime;
    int state;

};

