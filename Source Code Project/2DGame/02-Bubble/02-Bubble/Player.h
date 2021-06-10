#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "PlayerState.h"
#include <list>
#include "Portal.h"
#include "SoundManager.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, ShaderProgram& shaderProgram, bool& sceneChanged, vector<Portal>& portals);
	void updateState(ShaderProgram& shaderProgram);
	void render();
	void renderState();
	bool getDirection();

	bool hitting();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	pair <glm::ivec2, glm::ivec2> getBoundingBox();
	void checkForItems();
	void cheats();
	void increaseRescuedFriend();

	glm::ivec2 centroid();
	void increaseExperienceAndScore(int exp, int score);

	bool playerIsDamaged();
	bool hasHelmet();
	bool hasWaterProof();
	bool hasShield();
	bool hasClock();

	void treatPortals(vector<Portal>& portals);

	void setState(PlayerState* playerState);

	bool isUltimate();

	PlayerState* getState();

	bool isDead();
	bool hasWon();

	void activateHurtTile(bool value);

	glm::ivec2 getPosition() {
		return posPlayer;
	}

	glm::ivec2 getPlayerSize() {
		return playerSize;

	}

	glm::ivec2 getSideMargins() {
		return sideMargins;
	}

private:
	bool bJumping, bClimb, isDamaged, bAttack, key;
	glm::ivec2 tileMapDispl, posPlayer, playerSize, sideMargins;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	float delayTime;
	int state;
	int direction, ultiDirection;
	int ultiTimer;
	bool powerUpsObtained[5] = { false };
	PlayerState* playerState;
	int speedStep, clockTimer, currentTime;
	bool dead, win, godMode;

	void standGraphicsTreatment();
	void walkGraphicsTreatment();
	void updatePlayerDirection(const bool& leftB, const bool& rightB);
	void treatGraphics();
	ISoundEngine* engine;
};


#endif