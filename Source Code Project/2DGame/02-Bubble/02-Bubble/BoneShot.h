#ifndef _BONE_SHOT_INCLUDE
#define _BONE_SHOT_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class BoneShot
{

public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const int boneStep);
    bool update(int deltaTime, const glm::ivec2& cameraPosition);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);

    void initSprites(const glm::vec2& sizeInTiles, ShaderProgram& shaderProgram);
    void initSpriteSheet(const string filename);

    pair <glm::ivec2, glm::ivec2> getBoundingBox();

    void setImpact(bool b);

    glm::ivec2 getPosition() {
        return position;
    }

    glm::ivec2 getSize() {
        return size;
    }



private:
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;

    glm::ivec2 tileMapDispl, position, size;
    int currentTime, creationTime;
    int state, direction, step;
    bool impact;

};
#endif
