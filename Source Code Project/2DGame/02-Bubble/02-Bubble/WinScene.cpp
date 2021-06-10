#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "WinScene.h"
#include "Game.h"



#define SCREEN_X 0
#define SCREEN_Y 40



WinScene::WinScene(glm::ivec2 posPlayer)
{
	map = NULL;
}

WinScene::~WinScene()
{
	if (map != NULL)
		delete map;
}


void WinScene::init()
{
	initShaders();
	cameraPosition = glm::vec2(0, 0);
	map = TileMap::createTileMap("levels/GameComplete.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map->setTexProgram(texProgram);
	projection = glm::ortho(cameraPosition.x, cameraPosition.x + SCREEN_WIDTH - 1, cameraPosition.y + SCREEN_HEIGHT - 1, cameraPosition.y);
	currentTime = 0;
	startTime = 0;
	engine = SoundManager::instance().getSoundEngine();
	engine->removeAllSoundSources();
	engine->play2D("sounds/gameComplete.wav", false);
}


void WinScene::update(int deltaTime)
{
	currentTime += deltaTime;
	playerState->update(glm::ivec2(0, 0), glm::ivec2(0, 0), 640, 400, texProgram);
	if (currentTime - startTime >= 17000) Game::instance().changeScene(6, glm::ivec2(0, 0), nullptr);
}


void WinScene::render()
{
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	playerState->render();
}

void WinScene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void WinScene::setPlayerState(PlayerState* playerState) {
	this->playerState = playerState;
}