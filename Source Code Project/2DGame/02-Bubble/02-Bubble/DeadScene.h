#ifndef _DEAD_SCENE_INCLUDE
#define _DEAD_SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "FallingRock.h"
#include "WaterDrop.h"
#include "PlayerState.h"
#include "Friend.h"
#include "Fire.h"
#include "WalkingSkull.h"
#include "GeneralScene.h"
#include <set>
#include "Portal.h"
#include "SoundManager.h"





// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class DeadScene : public GeneralScene
{

public:
	DeadScene(glm::ivec2 posPlayer);
	~DeadScene();

	void init();
	void update(int deltaTime);
	void render();
	void setPlayerState(PlayerState* playerState);

private:
	void initShaders();

private:
	TileMap* map;
	PlayerState* playerState;
	ISoundEngine* engine;
	
	ShaderProgram texProgram;
	int currentTime, startTime;
	glm::mat4 projection;
	glm::vec2 cameraPosition, playerPosition;
	Friend* friendItem;
	float delayTime, delayRockCrashed;

};


#endif // _SCENE_INCLUDE

