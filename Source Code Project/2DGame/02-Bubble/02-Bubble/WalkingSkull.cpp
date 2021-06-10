#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "WalkingSkull.h"

#define SKULL_WALK_STEP 2
#define SHOT_SPEED 3

enum Direction {
	LEFT_D, RIGHT_D
};

enum State {
	SPAWNING, LOOK_ASIDE, WALK, SHOOT, DEATH
};

enum SkullAnimations {
	SPAWN, SIDE_TO_SIDE, WALK_LEFT, WALK_RIGHT
};

WalkingSkull::WalkingSkull() {
	shot = nullptr;
}

WalkingSkull::~WalkingSkull() {
	if (shot != nullptr)
		delete shot;
}

void WalkingSkull::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	direction = LEFT_D;
	sideMargins = glm::ivec2(10, 10);
	state = SPAWNING;
	size = glm::ivec2(40,40);
	position = glm::ivec2(0,0);
	tileMapDispl = tileMapPos;
	lastRandomGenerated = 0;
	currentTime = 0;
	creationTime = 0;
	initSpriteSheet("images/WalkingSkull.png");
	initSprites(glm::vec2(0.25f, 0.5f), shaderProgram);
}

void WalkingSkull::initSpriteSheet(const string filename) {
	spritesheet.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
}

void WalkingSkull::initSprites(const glm::vec2& sizeInTiles, ShaderProgram& shaderProgram) {
	sprite = Sprite::createSprite(size, sizeInTiles, &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(SPAWN, 3);
	sprite->addKeyframe(SPAWN, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(SPAWN, glm::vec2(0.75f, 0.f));

	sprite->setAnimationSpeed(SIDE_TO_SIDE, 3);
	sprite->addKeyframe(SIDE_TO_SIDE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(SIDE_TO_SIDE, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(WALK_LEFT, 8);
	sprite->addKeyframe(WALK_LEFT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(WALK_LEFT, glm::vec2(0.25f, 0.5f));
	
	sprite->setAnimationSpeed(WALK_RIGHT, 8);
	sprite->addKeyframe(WALK_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(WALK_RIGHT, glm::vec2(0.25f, 0.f));
	
	sprite->changeAnimation(SPAWN);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void WalkingSkull::update(int deltaTime, const glm::ivec2& playerPosition, ShaderProgram& shaderProgram, const glm::ivec2& cameraPosition) {
	currentTime += deltaTime;

	if (shot != nullptr) {
		if (shot->update(deltaTime, cameraPosition))
			shot = nullptr;
	}
		

	switch (state)
	{
	case SPAWNING:
		if (currentTime - creationTime > 1500) {
			state = LOOK_ASIDE;
			creationTime = currentTime;
		}
		break;
	case LOOK_ASIDE:
		if (currentTime - creationTime > 2000) {
			state = WALK;
		}

		int boneDirection;
		if (playerInRange(playerPosition, boneDirection)) {//bone creation
			if (shot == nullptr) {
				int shotStep;
				if (boneDirection == LEFT_D) shotStep = -SHOT_SPEED;
				else shotStep = SHOT_SPEED;
				shot = new BoneShot();
				shot->init(tileMapDispl, shaderProgram,shotStep);
				shot->setPosition(glm::vec2(float(position.x), float(position.y)));
				shot->setTileMap(map);
			}
		}
			
		break;
	case WALK:
		if (rand() % 10 == 5 && currentTime - lastRandomGenerated > 10000) {
			state = LOOK_ASIDE;
			creationTime = currentTime;
			lastRandomGenerated = currentTime;
			bool cont = (rand() % 10) > 4;
			if (direction == LEFT_D && cont) direction = RIGHT_D;
			else if (cont) direction = LEFT_D;
		}
		else {
			if (!map->enableToMoveLeft(position, size, cameraPosition)) {
				direction = RIGHT_D;
				state = LOOK_ASIDE;
				creationTime = currentTime;
			}
			else if (!map->enableToMoveRight(position, size, cameraPosition)) {
				direction = LEFT_D;
				state = LOOK_ASIDE;
				creationTime = currentTime;
			}

			if (map->enableToMoveLeft(position, size, cameraPosition) && direction == LEFT_D)
				position.x -= SKULL_WALK_STEP;
			else if (map->enableToMoveRight(position, size, cameraPosition) && direction == RIGHT_D)
				position.x += SKULL_WALK_STEP;
		}
		break;
	default:
		break;
	}
	updateGraphics(deltaTime);
	
}

bool WalkingSkull::playerInRange(const glm::ivec2& playerPosition, int& boneDirection) {
	int tileSize = map->getTileSize();
	int xTile = position.x / tileSize;
	int yTile = position.y / tileSize;
	glm::ivec2 skullTile = glm::ivec2(xTile,yTile);

	int xTilePlayer = playerPosition.x / tileSize;
	int yTilePlayer = playerPosition.y / tileSize; 
	glm::ivec2 playerTile = glm::ivec2(xTilePlayer, yTilePlayer);

	if (checkLeft(skullTile, playerTile,true, boneDirection)) return true;
	else if (checkLeft(skullTile, playerTile, false, boneDirection)) return true;
	return false;
}

bool WalkingSkull::checkLeft(const glm::ivec2& skullPosition, const glm::ivec2& playerPosition, const bool& left, int& boneDirection) {
	int x = skullPosition.x;
	int y = skullPosition.y;
	while (map->isTileIgnored(map->getNumTile(x,y))) {
		if (x == playerPosition.x && y == playerPosition.y) {
			if (left) boneDirection = LEFT_D;
			else boneDirection = RIGHT_D;
			return true;
		}
		if (left) --x;
		else ++x;
	}
	return false;
}

void WalkingSkull::updateGraphics(int deltaTime) {
	

	switch (state)
	{
	case SPAWNING:
		if (sprite->animation() != SPAWN)
			sprite->changeAnimation(SPAWN);
		break;
	case LOOK_ASIDE:
		if (sprite->animation() != SIDE_TO_SIDE)
			sprite->changeAnimation(SIDE_TO_SIDE);
		break;
	case WALK:
		if (direction == LEFT_D) {
			if (sprite->animation() != WALK_LEFT)
				sprite->changeAnimation(WALK_LEFT);
		}
		else {
			if (sprite->animation() != WALK_RIGHT)
				sprite->changeAnimation(WALK_RIGHT);
		}
		break;
	default:
		break;
	}
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void WalkingSkull::render() {
	if (shot != nullptr)
		shot->render();
	sprite->render();
}

void WalkingSkull::setTileMap(TileMap* tileMap) {
	map = tileMap;
}

void WalkingSkull::setPosition(const glm::vec2& pos) {
	position = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

pair <glm::ivec2, glm::ivec2> WalkingSkull::getBoundingBox() {

	glm::ivec2 x = glm::ivec2(position.x + sideMargins.y, position.x + size.x - 1 - sideMargins.x);
	glm::ivec2 y = glm::ivec2(position.y + sideMargins.y, position.y + size.y - 1 - sideMargins.y);

	return make_pair(x, y);
}

pair <glm::ivec2, glm::ivec2> WalkingSkull::getShotBoundingBox() {
	pair <glm::ivec2, glm::ivec2> result;
	if (shot != nullptr) result = shot->getBoundingBox();
	return result;
}

bool WalkingSkull::hasShot() {
	return shot != nullptr;
}

void WalkingSkull::setShotImpact() {
	shot->setImpact(true);
}