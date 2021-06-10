#ifndef _WALKING_SKULL_INCLUDE
#define _WALKING_SKULL_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "BoneShot.h"


class WalkingSkull
{

public:
    WalkingSkull();
    ~WalkingSkull();

    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime, const glm::ivec2& playerPosition, ShaderProgram& shaderProgram, const glm::ivec2& cameraPosition);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);

    void initSprites(const glm::vec2& sizeInTiles, ShaderProgram& shaderProgram);
    void initSpriteSheet(const string filename);
    void updateGraphics(int deltaTime);
    bool playerInRange(const glm::ivec2& playerPosition, int& boneDirection);
    bool checkLeft(const glm::ivec2& skullPosition, const glm::ivec2& playerPosition, const bool& left, int& boneDirection);

    bool hasShot();
    void setShotImpact();
    pair <glm::ivec2, glm::ivec2> getShotBoundingBox();

    pair <glm::ivec2, glm::ivec2> getBoundingBox();

    glm::ivec2 getPosition() {
        return position;
    }

    glm::ivec2 getEnemySize() {
        return size;
    }



private:
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
    BoneShot* shot;

    glm::ivec2 tileMapDispl, position, size, sideMargins;
    int currentTime, creationTime, lastRandomGenerated;
    int state, direction;

};
#endif