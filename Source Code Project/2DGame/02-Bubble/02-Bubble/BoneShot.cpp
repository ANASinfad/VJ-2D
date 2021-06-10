#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "BoneShot.h"

enum State {
	ROLL, CRASH
};

enum ShotAnimation {
	ROLLING, CRASHED
};

void BoneShot::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const int boneStep) {
	impact = false;
	state = ROLL;
	step = boneStep;
	size = glm::ivec2(25, 25);
	position = glm::ivec2(0, 0);
	tileMapDispl = tileMapPos;
	currentTime = 0;
	creationTime = 0;
	initSpriteSheet("images/bone_proyectile.png");
	initSprites(glm::vec2(0.25f, 1.f), shaderProgram);
}
bool BoneShot::update(int deltaTime, const glm::ivec2& cameraPosition) {
	bool eraseMyself = false;
	currentTime += deltaTime;
	bool canMoveLeft = step < 0 && map->enableToMoveLeft(glm::ivec2(position.x, position.y + size.y), size, cameraPosition);
	bool canMoveRight = step > 0 && map->enableToMoveRight(glm::ivec2(position.x, position.y + size.y), size, cameraPosition);
	switch (state)
	{
	case ROLL:
		if (!impact && (canMoveLeft || canMoveRight))
				position.x += step;	
		else {
			creationTime = currentTime;
			state = CRASH;
		}
		break;
	case CRASH:
		if (currentTime - creationTime > 250.f)
			eraseMyself = true;
		sprite->changeAnimation(CRASHED);
		break;
	default:
		break;
	}

	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	if (eraseMyself) delete this;
	return eraseMyself;
}
void BoneShot::render() {
	sprite->render();
}

void BoneShot::setTileMap(TileMap* tileMap) {
	map = tileMap;
}

void BoneShot::setPosition(const glm::vec2& pos) {
	position = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void BoneShot::initSprites(const glm::vec2& sizeInTiles, ShaderProgram& shaderProgram) {
	sprite = Sprite::createSprite(size, sizeInTiles, &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(ROLLING, 30);
	sprite->addKeyframe(ROLLING, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(ROLLING, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(ROLLING, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(CRASHED, 3);
	sprite->addKeyframe(CRASHED, glm::vec2(0.75f, 0.f));

	sprite->changeAnimation(ROLLING);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void BoneShot::initSpriteSheet(const string filename) {
	spritesheet.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
}

pair <glm::ivec2, glm::ivec2> BoneShot::getBoundingBox() {
	glm::ivec2 x = glm::ivec2(position.x, position.x + size.x - 1);
	glm::ivec2 y = glm::ivec2(position.y, position.y + size.y - 1);

	return make_pair(x, y);
}

void BoneShot::setImpact(bool b) {
	impact = b;
}