#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
#include "Scene5.h"
#include "WinScene.h"
#include "DeadScene.h"
#include "IntroScene.h"
#include "GeneralScene.h"
#include "SceneManager.h"
#include "CreditsScene.h"
#include "SoundManager.h"
#include "instruScene.h"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define LEVEL_SIZE_X 640
#define LEVEL_SIZE_Y 400


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	void changeScene(int value, glm::ivec2 posPlayer, PlayerState * playerState);
	void clearLevels();
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

private:
	Scene* scene;
	Scene2* scene2;
	Scene3* scene3;
	Scene4* scene4;
	Scene5* scene5;
	bool bPlay;                       // Continue to play game?
									  // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	SceneManager sceneManager;
	ISoundEngine* engine;

};


#endif // _GAME_INCLUDE


