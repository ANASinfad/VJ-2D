#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include "PlayerState.h"


#define TILESHEET_WIDTH 31
#define TILESHEET_HEIGHT 5
#define TILESIZE 20

#define STATS_WIDTH 32
#define STATS_HEIGHT 2

#define LIFE_BAR_START 15
#define EXPERIENCE_BAR_START 47
#define SCORE_BAR_START 4
#define CURRENT_SCORE_BAR_START 36
#define RESCUED_BAR_START 25

#define UP_SIDE_START_X 0
#define UP_SIDE_START_Y 0

#define DOWN_SIDE_START_X 0
#define DOWN_SIDE_START_Y 440

#define MAX_BAR_SIZE 10
#define MAX_RESCUED_FRIENDS 6

#define HELMET_TILE 94
#define WATERPROOF_TILE 96
#define SHIELD_TILE 98
#define HYPERSHOES_TILE 100
#define CLOCK_TILE 102

enum POWER_UPS {
	HELMET, WATER_PROOF, SHIELD, HYPER_SHOES, CLOCK
};

void PlayerState::showItem(int item) {
	if (item < 5 && !powerups[item] ) {
		switch (item)
		{
		case HELMET:
			updateItem(HELMET_TILE);
			powerups[0] = true;
			break;
		case WATER_PROOF:
			updateItem(WATERPROOF_TILE);
			powerups[1] = true;
			break;
		case SHIELD:
			updateItem(SHIELD_TILE);
			powerups[2] = true;
			break;
		case HYPER_SHOES:
			updateItem(HYPERSHOES_TILE);
			powerups[3] = true;
			break;
		case CLOCK:
			updateItem(CLOCK_TILE);
			powerups[4] = true;
			break;
		default:
			increasePlayerExperience(MAX_BAR_SIZE);
			increasePlayerScore(1000);
			break;
		}
	}
}

void PlayerState::updateItem(int itemPositionInTilesheet) {
	for (int i = 0; i < 2; ++i) {
		for (int j = nextPowerUpPosition; j < nextPowerUpPosition + 2; ++j) {
			itemStats[i * STATS_WIDTH + j] = itemPositionInTilesheet;
			itemPositionInTilesheet++;
		}
		itemPositionInTilesheet += 29;
	}
	nextPowerUpPosition += 2;
}

void PlayerState::init(const glm::ivec2& tileMapPosUP, const glm::ivec2& tileMapPosDOWN, ShaderProgram& shaderProgram) {
	playerLife = MAX_BAR_SIZE;
	playerExperience = 0;
	playerScene = 0;
	playerLevel = 0;
	playerScore = 0;
	key = false;
	godMode = false;
	rescuedFriends = 0;
	cameraPosition = glm::ivec2(0, 0);
	nextPowerUpPosition = 0;
	spritesheet.loadFromFile("images/state_player_v2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	lifeStats = new int[STATS_WIDTH * STATS_HEIGHT];
	itemStats = new int[STATS_WIDTH * STATS_HEIGHT];
	tileTexSize = glm::vec2(1.f / TILESHEET_WIDTH, 1.f / TILESHEET_HEIGHT);
	initUpSideBar();
	initDownSideBar();
	prepare(shaderProgram, UP_SIDE_START_X, UP_SIDE_START_Y, lifeStats, vaoUP, vboUP, posLocationUP, texCoordLocationUP);
	prepare(shaderProgram, DOWN_SIDE_START_X, DOWN_SIDE_START_Y, itemStats, vaoDOWN, vboDOWN, posLocationDOWN, texCoordLocationDOWN);
}

void PlayerState::initUpSideBar() {
	for (int i = 0; i < STATS_HEIGHT; ++i)
		for (int j = 0; j < STATS_WIDTH; ++j)
			lifeStats[j + i * STATS_WIDTH] = 13;
	lifeStats[1] = 40; lifeStats[2] = 41; lifeStats[3] = 32;//HI -
	lifeStats[11] = 54; lifeStats[12] = 41; lifeStats[13] = 52; lifeStats[14] = 68; lifeStats[25] = 67;//VIT
	lifeStats[33] = 2; lifeStats[34] = 48; lifeStats[35] = 32;//1P -
	lifeStats[43] = 37; lifeStats[44] = 56; lifeStats[45] = 48; lifeStats[46] = 68; lifeStats[57] = 67;// EXP
	lifeStats[26] = 51; lifeStats[27] = 35; lifeStats[28] = 37; lifeStats[29] = 46; lifeStats[30] = 37;//SCENE
	lifeStats[60] = 32; //-
	updateLifeBar();
	updateExperienceBar();
	updateScoreBar();
}

void PlayerState::initDownSideBar() {
	for (int i = 0; i < STATS_HEIGHT; ++i)
		for (int j = 0; j < STATS_WIDTH; ++j) {
			if (i == 0) {
				if (j == 24) itemStats[j + i * STATS_WIDTH] = 77;
				else if (j == 23) itemStats[j + i * STATS_WIDTH] = 73;
				else if (24 < j && j < 31) itemStats[j + i * STATS_WIDTH] = 75;
				else itemStats[j + i * STATS_WIDTH] = 72;
			}
			else {
				itemStats[j + i * STATS_WIDTH] = 13;
			}

		}
}

void PlayerState::increasePlayerScore(int quantity) {
	if (playerScore + quantity <= 999999)
		playerScore += quantity;
	else playerScore = 999999;
	updateScoreBar();
}

int PlayerState::increaseRescuedFriends() {
	rescuedFriends++;
	updateRescuedFriends();
	return rescuedFriends;
}

int PlayerState::increasePlayerLife(int quantity) {
	if (playerLife + quantity <= MAX_BAR_SIZE)
		playerLife += quantity;
	updateLifeBar();
	if (playerLife <= 0) return 0;
	return playerLife;
}

void PlayerState::increasePlayerExperience(int quantity) {
	if ((playerExperience + quantity) >= MAX_BAR_SIZE)
		increasePlayerLife(1);
	playerExperience = (playerExperience + quantity) % MAX_BAR_SIZE;
	updateExperienceBar();
}

void PlayerState::updateLifeBar() {
	for (int i = 0; i < MAX_BAR_SIZE; ++i) {
		if (i < playerLife) lifeStats[LIFE_BAR_START + i] = 63;
		else lifeStats[LIFE_BAR_START + i] = 70;
	}
}

void PlayerState::updateExperienceBar() {
	for (int i = 0; i < MAX_BAR_SIZE; ++i) {
		if (i < playerExperience) lifeStats[EXPERIENCE_BAR_START + i] = 65;
		else lifeStats[EXPERIENCE_BAR_START + i] = 70;
	}
}

void PlayerState::updateScoreBar() {
	string num = to_string(playerScore);
	int j = 0;
	for (int i = 0; i < 6; ++i) {
		if (i < 6 - num.length()) {
			lifeStats[SCORE_BAR_START + i] = 1;
			lifeStats[CURRENT_SCORE_BAR_START + i] = 1;
		}
		else {
			lifeStats[SCORE_BAR_START + i] = num[j] - '0' + 1;
			lifeStats[CURRENT_SCORE_BAR_START + i] = num[j] - '0' + 1;
			++j;
		}
	}
}

void PlayerState::updateRescuedFriends() {
	for (int i = 0; i < MAX_RESCUED_FRIENDS; ++i) {
		if (i < rescuedFriends) itemStats[RESCUED_BAR_START + i] = 73;
		else itemStats[RESCUED_BAR_START + i] = 75;
	}
}

void PlayerState::prepare(ShaderProgram& shaderProgram, int xOffset, int yOffset, int* sideMap, GLuint& vao, GLuint& vbo, GLint& posLocation, GLint& texCoordLocation) {
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	halfTexel = glm::vec2(0.5f / spritesheet.width(), 0.5f / spritesheet.height());
	for (int j = 0; j < STATS_HEIGHT; j++)
	{
		for (int i = 0; i < STATS_WIDTH; i++)
		{
			tile = sideMap[j * STATS_WIDTH + i];
			if (tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(i * TILESIZE, j * TILESIZE);
				texCoordTile[0] = glm::vec2(float((tile - 1) % TILESHEET_WIDTH) / TILESHEET_WIDTH, float((tile - 1) / TILESHEET_WIDTH) / TILESHEET_HEIGHT);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x + cameraPosition.x + xOffset); vertices.push_back(posTile.y + cameraPosition.y + yOffset);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + TILESIZE + cameraPosition.x + xOffset); vertices.push_back(posTile.y + cameraPosition.y + yOffset);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + TILESIZE + cameraPosition.x + xOffset); vertices.push_back(posTile.y + TILESIZE + cameraPosition.y + yOffset);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x + cameraPosition.x + xOffset); vertices.push_back(posTile.y + cameraPosition.y + yOffset);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + TILESIZE + cameraPosition.x + xOffset); vertices.push_back(posTile.y + TILESIZE + cameraPosition.y + yOffset);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x + cameraPosition.x + xOffset); vertices.push_back(posTile.y + TILESIZE + cameraPosition.y + yOffset);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = shaderProgram.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = shaderProgram.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void PlayerState::update(const glm::ivec2& pp, const glm::ivec2& pSize, const int width, const int height, ShaderProgram& shaderProgram) {
	if (pp.x > cameraPosition.x + width) {
		cameraPosition.x += width;
	}
	else if (pp.x < cameraPosition.x) {
		cameraPosition.x -= width;
	}

	if (pp.y + pSize.y > cameraPosition.y + height) {
		cameraPosition.y += height;
	}
	else if (pp.y + pSize.y < cameraPosition.y) {
		cameraPosition.y -= height;
	}
	prepare(shaderProgram, UP_SIDE_START_X, UP_SIDE_START_Y, lifeStats, vaoUP, vboUP, posLocationUP, texCoordLocationUP);
	prepare(shaderProgram, DOWN_SIDE_START_X, DOWN_SIDE_START_Y, itemStats, vaoDOWN, vboDOWN, posLocationDOWN, texCoordLocationDOWN);
}

void PlayerState::render() {
	glEnable(GL_TEXTURE_2D);
	spritesheet.use();
	glBindVertexArray(vaoUP);
	glEnableVertexAttribArray(posLocationUP);
	glEnableVertexAttribArray(texCoordLocationUP);
	glDrawArrays(GL_TRIANGLES, 0, 6 * TILESHEET_WIDTH * TILESHEET_HEIGHT);
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	spritesheet.use();
	glBindVertexArray(vaoDOWN);
	glEnableVertexAttribArray(posLocationDOWN);
	glEnableVertexAttribArray(texCoordLocationDOWN);
	glDrawArrays(GL_TRIANGLES, 0, 6 * TILESHEET_WIDTH * TILESHEET_HEIGHT);
	glDisable(GL_TEXTURE_2D);
}

void PlayerState::clearClock() {
	if (powerups[4]) {
		int initTile = 0;
		bool stop = false;
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 10 && !stop; ++j) {
				int tile = itemStats[i * STATS_WIDTH + j];
				if (tile == 102) {
					initTile = j;
					stop = true;
				}
			}
		}
		for (int i = 0; i < 2; ++i) {
			for (int j = initTile; j < 10; ++j) {
				itemStats[i * STATS_WIDTH + j] = itemStats[i * STATS_WIDTH + j + 2];
			}
		}
		nextPowerUpPosition -= 2;
		powerups[4] = false;
	}
}

void PlayerState::setKey(bool b) {
	key = b;
	if (key)
		itemStats[RESCUED_BAR_START + 30] = 78;
	else
		itemStats[RESCUED_BAR_START + 30] = 13;
}

bool PlayerState::hasKey() {
	return key;
}

bool PlayerState::hasHelmet() {
	return powerups[0];
}

bool PlayerState::hasWaterProof() {
	return powerups[1];
}

bool PlayerState::hasHypershoes() {
	return powerups[3];
}

bool PlayerState::hasShield() {
	return powerups[2];
}

bool PlayerState::hasClock() {
	return powerups[4];
}

void PlayerState::setGodMode() {
	godMode = true;
}

bool PlayerState::isGodMode() {
	return godMode;
}
