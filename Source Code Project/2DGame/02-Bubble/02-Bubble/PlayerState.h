#ifndef _PLAYER_STATE_
#define _PLAYER_STATE_


#include "Sprite.h"
#include "TileMap.h"
#include "Texture.h"
#include "ShaderProgram.h"

class PlayerState
{

public:
	void showItem(int item);
	void init(const glm::ivec2& tileMapPosUP, const glm::ivec2& tileMapPosDOWN, ShaderProgram& shaderProgram);
	void update(const glm::ivec2& pp, const glm::ivec2& pSize, const int width, const int height, ShaderProgram& shaderProgram);
	void render();
	void prepare(ShaderProgram& shaderProgram, int xOffset, int yOffset, int* sideMap, GLuint& vao, GLuint& vbo, GLint& posLocation, GLint& texCoordLocation);
	//void prepareDownSide(ShaderProgram& shaderProgram);
	void updateLifeBar();
	void updateExperienceBar();
	void updateScoreBar();
	void updateRescuedFriends();
	int increasePlayerLife(int quantity);
	void increasePlayerExperience(int quantity);
	void increasePlayerScore(int quantity);
	int increaseRescuedFriends();
	void initUpSideBar();
	void initDownSideBar();
	void updateItem(int itemPositionInTilesheet);
	void clearClock();

	void setKey(bool b);

	bool hasKey();
	bool hasHelmet();

	bool hasWaterProof();
	bool hasClock();

	void setGodMode();

	bool isGodMode();

	bool hasHypershoes();

	bool hasShield();

private:
	GLuint vaoUP;
	GLuint vboUP;
	GLint posLocationUP, texCoordLocationUP;

	GLuint vaoDOWN;
	GLuint vboDOWN;
	GLint posLocationDOWN, texCoordLocationDOWN;

	Texture spritesheet;
	Sprite* sprite;
	int* lifeStats;
	int* itemStats;
	glm::vec2 tileTexSize;
	glm::ivec2 cameraPosition;
	bool key, godMode;
	int playerLife, playerExperience, playerScene, playerLevel, playerScore, rescuedFriends, nextPowerUpPosition;
	bool powerups[5] = { false };

};

#endif