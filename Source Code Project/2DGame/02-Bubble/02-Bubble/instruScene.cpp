#include "instruScene.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include "Game.h"

#define SCREEN_X 0
#define SCREEN_Y 0


instruScene::instruScene()
{
	map = NULL;
}

instruScene::~instruScene()
{
	if (map != NULL)
		delete map;
}


void instruScene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/instruction.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map->setTexProgram(texProgram);
	cameraPosition = glm::vec2(0, 0);
	projection = glm::ortho(cameraPosition.x, cameraPosition.x + SCREEN_WIDTH - 1, cameraPosition.y + SCREEN_HEIGHT - 1, cameraPosition.y);
	currentTime = 0.0f;
	engine = SoundManager::instance().getSoundEngine();
}

void instruScene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (Game::instance().getKey(' ')) {
		Game::instance().changeScene(1, glm::ivec2(10, 5), nullptr);
		engine = SoundManager::instance().getSoundEngine();
		engine->removeAllSoundSources();
		engine->play2D("sounds/GooniesGamplay.wav", true);
	}
	if (Game::instance().getKey('x')) Game::instance().changeScene(-1, glm::ivec2(0,0), nullptr);
}

void instruScene::render()
{
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
}

void instruScene::initShaders()
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