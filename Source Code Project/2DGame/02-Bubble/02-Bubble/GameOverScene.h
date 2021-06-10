#ifndef _GAME_OVER_SCENE_INCLUDE
#define _GAME_OVER_SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Scene.h"
#include <sstream>




// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene : public Scene
{

public:
	Scene(string sceneName);
	~Scene();

private:
	void initShaders();

private:
	string levelName;
};


#endif // _SCENE_INCLUDE

