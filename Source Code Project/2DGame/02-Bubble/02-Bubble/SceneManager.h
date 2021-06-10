#ifndef _SCENE_MANAGER_INCLUDE
#define _SCENE_MANAGER_INCLUDE


#include "GeneralScene.h"
#include "PlayerState.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application


class SceneManager
{

public:
	SceneManager() {
		currentScene = nullptr;
	}

	void init(PlayerState* playerState);
	void update(int deltaTime);
	void render();

	void changeScene(GeneralScene* scene, glm::ivec2 pposition, bool existed);

private:
	GeneralScene* currentScene;                      // Scene to render

};


#endif // SCENE_MANAGER includes
