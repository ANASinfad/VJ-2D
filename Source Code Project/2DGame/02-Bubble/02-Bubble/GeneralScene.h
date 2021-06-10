#ifndef _GENERAL_SCENE_INCLUDE_
#define _GENERAL_SCENE_INCLUDE_

#include"PlayerState.h"

// Game is a singleton (a class with a single instance) that represents our whole application


class GeneralScene
{

public:

	virtual void init() {};
	virtual void update(int deltaTime) {};
	virtual void render() {};
	virtual void setPlayerState(PlayerState* playerState) {};
	virtual void setPlayerPosition(glm::ivec2 pposition) {};

};


#endif // SCENE_MANAGER includes