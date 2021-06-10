#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "CreditsScene.h"

CreditsScene::CreditsScene(string name)
{
	textureName = name;
}

CreditsScene::~CreditsScene()
{
	if (texQuad != nullptr)
		delete texQuad;
}


void CreditsScene::init()
{
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	initShaders();
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	texQuad = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texs.loadFromFile("images/" + textureName, TEXTURE_PIXEL_FORMAT_RGBA);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	engine = SoundManager::instance().getSoundEngine();
	engine->removeAllSoundSources();
	engine->play2D("sounds/Goonies_Darkcave.wav", true);
	currentTime = 0;
	startTime = 0;
}

void CreditsScene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (currentTime - startTime >= 91000) Game::instance().changeScene(-1, glm::ivec2(0, 0), nullptr);
}

void CreditsScene::render()
{
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad->render(texs);
}

void CreditsScene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleProgram.init();
	simpleProgram.addShader(vShader);
	simpleProgram.addShader(fShader);
	simpleProgram.link();
	if (!simpleProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleProgram.log() << endl << endl;
	}
	simpleProgram.bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();
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
}

