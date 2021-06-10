#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Enemy.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define SPEED_STEP 3


enum PlayerAnims
{
	MOVE_LEFT, MOVE_RIGHT,IDLE
};


void Enemy::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	sideMargins = glm::ivec2(13, 13);
	bJumping = false;
	
	currentTime = 0.f;
	enemySize = glm::ivec2(40.f, 40.f);
	spritesheet.loadFromFile("images/skull.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	// 32 * 32 es el tamaño del tile
	// nos movemos con 0.2f
	sprite = Sprite::createSprite(glm::ivec2(30.f, 30.f), glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(IDLE, 9);
	sprite->addKeyframe(IDLE, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.75f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.f));

	sprite->changeAnimation(IDLE);
	

	
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
	delayTime = 1000.f;
	
	left = true;
}

void Enemy::update(int deltaTime, const glm::ivec2& playerPosition, ShaderProgram& shaderProgram, glm::ivec2 cameraPosition)
{
	sprite->update(deltaTime);
	if (delayTime <= 0) {
		
		if (left) {
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			posEnemy.x -= SPEED_STEP;
		}
		if (!left) {
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			posEnemy.x += SPEED_STEP;
		}
		if (!map->enableToMoveLeft(posEnemy, glm::ivec2(32, 32), cameraPosition) && left) {
			left = false;
			posEnemy.x += SPEED_STEP;
		}
		if (!map->enableToMoveRight(posEnemy, glm::ivec2(32, 32), cameraPosition) && !left) {
			left = true;
			posEnemy.x -= SPEED_STEP;

		}
		
	}
	else {
		if (sprite -> animation() != IDLE)
			sprite->changeAnimation(IDLE);
		delayTime -= deltaTime;
	} 
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));

}

void Enemy::render()
{
	sprite->render();
}

void Enemy::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Enemy::setPosition(const glm::vec2& pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

pair <glm::ivec2, glm::ivec2> Enemy::getBoundingBox() {

	glm::ivec2 x = glm::ivec2(posEnemy.x + sideMargins.y, posEnemy.x + enemySize.x -1 - sideMargins.x);
	glm::ivec2 y = glm::ivec2(posEnemy.y + sideMargins.y, posEnemy.y + enemySize.y -1 - sideMargins.y);

	return make_pair(x, y);
}