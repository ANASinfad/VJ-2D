#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.
// put some enemy description...


class Enemy 
{

public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime, const glm::ivec2& playerPosition, ShaderProgram& shaderProgram, glm::ivec2 cameraPosition);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);

    pair<glm::ivec2, glm::ivec2> getBoundingBox();

    glm::ivec2 getPosition() {
        return posEnemy;
    }

    glm::ivec2 getEnemySize() {
        return enemySize;
    }



private:
    bool bJumping, left;
    glm::ivec2 tileMapDispl, posEnemy, enemySize;
    int jumpAngle, startY;
    int currentTime;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
    glm::ivec2 sideMargins;
    float delayTime;
   
};
#endif // _ENEMY_INCLUDE