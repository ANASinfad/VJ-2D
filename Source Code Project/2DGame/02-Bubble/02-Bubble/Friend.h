#ifndef _FRIEND_INCLUDE
#define _FRIEND_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

class Friend {
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();
	void setPickedUp();
	bool isPickedUp();
	void setPosition(const glm::ivec2 initPos);

	glm::ivec2 getSize() {
		return FriendSize;
	}

	glm::ivec2 getFriendPos() {
		return FriendPos;
	}

	pair <glm::ivec2, glm::ivec2> getBoundingBox();


private:
	glm::ivec2 tileMapDispl, FriendPos, FriendSize;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	bool pickedUp;

};
#endif // _FRIEND_INCLUDE