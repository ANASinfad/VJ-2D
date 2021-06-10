#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	GeneralScene *gs = new GeneralScene();
	IntroScene *introScene = new IntroScene();
	SoundManager::instance().init();
	engine = SoundManager::instance().getSoundEngine();
	engine->play2D("sounds/Goonies_Goodenough.wav", true);
	gs = introScene;
	sceneManager.changeScene(gs, glm::ivec2(0, 0), false);
	sceneManager.init(nullptr);
	scene = nullptr;
	scene2 = nullptr;
	scene3 = nullptr;
	scene4 = nullptr;
	scene5 = nullptr;
}

bool Game::update(int deltaTime)
{

	if (getKey('1')) {
		clearLevels();
		engine->removeAllSoundSources();
		engine->play2D("sounds/Goonies_Goodenough.wav", true);
		IntroScene* scene = new IntroScene();
		GeneralScene* gs = new GeneralScene();
		gs = scene;
		sceneManager.changeScene(gs, glm::ivec2(0, 0), false);
		sceneManager.init(nullptr);
	}
	else if (getKey('2')) {
		clearLevels();
		scene = new Scene(glm::ivec2 ( 10,5));
		GeneralScene* gs = new GeneralScene();
		gs = scene;
		sceneManager.changeScene(gs, glm::ivec2(0, 0), false);
		sceneManager.init(nullptr);
		engine->removeAllSoundSources();
		engine->play2D("sounds/GooniesGamplay.wav", true);
	}
	else if (getKey('3')) {
		clearLevels();
		scene2 = new Scene2(glm::ivec2(24,9));
		GeneralScene* gs = new GeneralScene();
		gs = scene2;
		sceneManager.changeScene(gs, glm::ivec2(0, 0), false);
		sceneManager.init(nullptr);
		engine->removeAllSoundSources();
		engine->play2D("sounds/GooniesGamplay.wav", true);
	}
	else if (getKey('4')) {
		clearLevels();
		scene3 = new Scene3(glm::ivec2(3, 5));
		GeneralScene* gs = new GeneralScene();
		gs = scene3;
		sceneManager.changeScene(gs, glm::ivec2(0, 0), false);
		sceneManager.init(nullptr);
		engine->removeAllSoundSources();
		engine->play2D("sounds/GooniesGamplay.wav", true);
	}
	else if (getKey('5')) {
		clearLevels();
		scene4 = new Scene4(glm::ivec2(21, 4));
		GeneralScene* gs = new GeneralScene();
		gs = scene4;
		sceneManager.changeScene(gs, glm::ivec2(0, 0), false);
		sceneManager.init(nullptr);
		engine->removeAllSoundSources();
		engine->play2D("sounds/GooniesGamplay.wav", true);
	}
	else if (getKey('6')) {
		clearLevels();
		scene5 = new Scene5(glm::ivec2(6, 7));
		GeneralScene* gs = new GeneralScene();
		gs = scene5;
		sceneManager.changeScene(gs, glm::ivec2(0, 0), false);
		sceneManager.init(nullptr);
		engine->removeAllSoundSources();
		engine->play2D("sounds/GooniesGamplay.wav", true);
		
	}
	else if (getKey('7')) {
		clearLevels();
		CreditsScene* scene = new CreditsScene("creditos_goonies.png");
		GeneralScene* gs = new GeneralScene();
		gs = scene;
		sceneManager.changeScene(gs, glm::ivec2(0, 0), false);
		sceneManager.init(nullptr);
		engine->removeAllSoundSources();
		engine->play2D("sounds/Goonies_Darkcave.wav", true);
	}
	sceneManager.update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	sceneManager.render();
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::changeScene(int value, glm::ivec2 posPlayer, PlayerState * playerState) {
	GeneralScene* gs = new GeneralScene();
	if (value == 1) {
		bool existed = false;
		if (scene == nullptr) scene = new Scene(posPlayer);
		else {
			scene->setPlayerPosition(posPlayer);
			existed = true;
		}
		gs = scene;
		sceneManager.changeScene(gs, posPlayer, existed);
		if (!existed) sceneManager.init(playerState);
	}
	else if (value == 2) {
		bool existed = false;
		if (scene2 == nullptr) scene2 = new Scene2(posPlayer);
		else {
			scene2->setPlayerPosition(posPlayer);
			existed = true;
		}
		gs = scene2;
		sceneManager.changeScene(gs, posPlayer, existed);
		if (!existed) sceneManager.init(playerState);
	}
	else if (value == 3) {
		bool existed = false;
		if (scene3 == nullptr) scene3 = new Scene3(posPlayer);
		else {
			scene3->setPlayerPosition(posPlayer);
			existed = true;
		}
		gs = scene3;
		sceneManager.changeScene(gs, posPlayer, existed);
		if (!existed) sceneManager.init(playerState);
	}
	else if (value == 4) {
		bool existed = false;
		if (scene4 == nullptr) scene4 = new Scene4(posPlayer);
		else {
			scene4->setPlayerPosition(posPlayer);
			existed = true;
		}
		gs = scene4;
		sceneManager.changeScene(gs, posPlayer, existed);
		if (!existed) sceneManager.init(playerState);
	}
	else if (value == 5){
		bool existed = false;
		if (scene5 == nullptr) scene5 = new Scene5(posPlayer);
		else {
			scene5->setPlayerPosition(posPlayer);
			existed = true;
		}
		gs = scene5;
		sceneManager.changeScene(gs, posPlayer, existed);
		if (!existed) sceneManager.init(playerState);
	} 
	else if (value == 6) {
		CreditsScene* scene = new CreditsScene("creditos_goonies.png");
		gs = scene;
		sceneManager.changeScene(gs, glm::ivec2(0, 0), false);
		sceneManager.init(playerState);
	}
	else if (value == 7) {
		DeadScene* scene = new DeadScene(glm::ivec2(0,0));
		gs = scene;
		sceneManager.changeScene(gs, glm::ivec2(0, 0), false);
		sceneManager.init(playerState);
	}
	else if (value == 8) {
		WinScene* scene = new WinScene(glm::ivec2(0, 0));
		gs = scene;
		sceneManager.changeScene(gs, glm::ivec2(0, 0), false);
		sceneManager.init(playerState);
	}
	else if ( value == -1 ) {
		clearLevels();
		engine->removeAllSoundSources();
		init();
		/*engine->removeAllSoundSources();
		engine->play2D("sounds/Goonies_Goodenough.wav", true);
		IntroScene* scene = new IntroScene();
		GeneralScene* gs = new GeneralScene();
		gs = scene;
		sceneManager.changeScene(gs,glm::ivec2(0,0),false);
		sceneManager.init(nullptr)*/;
	}
	else {
		instruScene* scene = new instruScene();
		GeneralScene* gs = new GeneralScene();
		gs = scene;
		sceneManager.changeScene(gs, glm::ivec2(0, 0), false);
		sceneManager.init(nullptr);
	}
}

void Game::clearLevels() {
	if (scene != nullptr) {
		delete scene;
		scene = nullptr;
	}
	if (scene2 != nullptr) {
		delete scene2;
		scene2 = nullptr;
	}
	if (scene3 != nullptr) {
		delete scene3;
		scene3 = nullptr;
	}
	if (scene4 != nullptr) {
		delete scene4;
		scene4 = nullptr;
	}
	if (scene5 != nullptr) {
		delete scene5;
		scene5 = nullptr;
	}
}