#ifndef _GENERAL_ENEMY_INCLUDE_
#define _GENERAL_ENEMY_INCLUDE_
#include <cmath>

#include "TileMap.h"
#include "Sprite.h"
#include "Game.h"



// Game is a singleton (a class with a single instance) that represents our whole application


class GeneralEnemy
{

public:

	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime, const glm::ivec2& playerPosition, ShaderProgram& shaderProgram);
    virtual void render();
    
    glm::ivec2 getPosition() {
        return posEnemy;
    }

    glm::ivec2 getEnemySize() {
        return enemySize;
    }

    void setTileMap(TileMap* tileMap)
    {
	    map = tileMap;
    }

    void setPosition(const glm::vec2& pos)
    {
	    posEnemy = pos;
	    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
    }

    pair <glm::ivec2, glm::ivec2> getBoundingBox() {
	    glm::ivec2 x = glm::ivec2(posEnemy.x + sideMargins.y, posEnemy.x + enemySize.x -1 - sideMargins.x);
	    glm::ivec2 y = glm::ivec2(posEnemy.y + sideMargins.y, posEnemy.y + enemySize.y -1 - sideMargins.y);
	    return make_pair(x, y);
    }


protected:
    glm::ivec2 tileMapDispl, posEnemy, enemySize;
    int currentTime;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
    glm::ivec2 sideMargins;
};


#endif // SCENE_MANAGER includes