#pragma once

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "GeneralScene.h"
#include "SoundManager.h"
class instruScene : public GeneralScene
{

public:
	instruScene();
	~instruScene();

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

