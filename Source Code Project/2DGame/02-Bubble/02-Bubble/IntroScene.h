#ifndef _INTRO_SCENE_INCLUDE
#define _INTRO_SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "GeneralScene.h"
#include "SoundManager.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class IntroScene : public GeneralScene
{

public:
	IntroScene();
	~IntroScene();

	void init();
	void update(int deltaTime);
	void render();
	void setPlayerState(PlayerState* playerState) {}

private:
	void initShaders();

private:
	TileMap* map;

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	glm::vec2 cameraPosition;
	ISoundEngine* engine;
};


#endif // _SCENE_INCLUDE