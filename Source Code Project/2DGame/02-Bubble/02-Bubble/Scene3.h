#ifndef _SCENE_3_INCLUDE
#define _SCENE_3_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "FallingRock.h"
#include "WaterDrop.h"
#include "PlayerState.h"
#include "Friend.h"
#include "Fire.h"
#include "WalkingSkull.h"
#include "GeneralScene.h"
#include <set>
#include "Portal.h"

class Scene3 : public GeneralScene
{
public:
	Scene3(glm::ivec2 posPlayer);
	~Scene3();

	void init();
	void update(int deltaTime);
	void updateEnemies(int deltaTime, glm::ivec2 playerPosition);
	void render();
	void renderEnemies();
	void updateCamera();
	void refreshObjects();
	void checkIfatackedByEnemy();
	bool isOverlapping1D(glm::ivec2 box1, glm::ivec2 box2);
	bool checkBoxOverlapping(const pair<glm::ivec2, glm::ivec2>& box1, const pair<glm::ivec2, glm::ivec2>& box2);
	void checkPickedUpItems();
	void checkIfRockDamaged(int deltaTime);
	void checkIfWatterDropDamaged();
	void checkIfRockHitTheGround(int deltaTime);
	void checkIFplayerNextToFire();
	void checkIfDamagedByFire();
	void updatePlayer(int deltaTime, ShaderProgram& shaderProgram, bool& sceneChanged);
	void checkKilledEnemies();
	void clearEnemies();
	void initEnemies(vector<pair<glm::ivec2, int>>& enemiesTiles);
	void initPortals(vector< glm::ivec2>& posPortals);
	void initPlayer();
	void setPlayerState(PlayerState* playerState);

	void setPlayerPosition(glm::ivec2 pposition);



private:
	void initShaders();

private:
	TileMap* map;
	Player* player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	glm::vec2 cameraPosition, playerPosition;
	Friend* friendItem;
	float delayTime;
	list<Enemy*> enemies;
	list<WalkingSkull*> Wenemies;
	list<WaterDrop* > waterObstacles;
	list<FallingRock* > rockObstacles;
	list<Fire* > fireObstacles;
	vector<Portal> portals;
};
#endif // _SCENE_INCLUDE

