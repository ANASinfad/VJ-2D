#ifndef _CREDITS_SCENE_INCLUDE
#define _CREDITS_SCENE_INCLUDE

#include <glm/glm.hpp>
#include "TexturedQuad.h"
#include "ShaderProgram.h"
#include "Quad.h"
#include "Game.h"
#include "SoundManager.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class CreditsScene : public GeneralScene
{

public:
	CreditsScene(string name);
	~CreditsScene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	ISoundEngine* engine;
	Texture texs;
	TexturedQuad* texQuad;
	ShaderProgram simpleProgram, texProgram;
	int currentTime, startTime;
	glm::mat4 projection;
	string textureName;
};
#endif