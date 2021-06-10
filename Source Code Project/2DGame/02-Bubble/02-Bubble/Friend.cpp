#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Friend.h"
#include "Game.h"


#define ITEM_WIDTH 20
#define ITEM_HEIGHT 40


enum FriendAnims{
	STAND_FRIEND
};

void Friend::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	pickedUp = false;
	FriendSize = glm::ivec2(ITEM_WIDTH, ITEM_HEIGHT);
	spritesheet.loadFromFile("images/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	// 32 * 32 es el tamaño del tile
	// nos movemos con 0.2f
	sprite = Sprite::createSprite(glm::ivec2(FriendSize.x, FriendSize.y), glm::vec2(0.2f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(STAND_FRIEND, 8);
	sprite->addKeyframe(STAND_FRIEND, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(STAND_FRIEND);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + FriendPos.x), float(tileMapDispl.y + FriendPos.y)));
}

void Friend::update(int deltaTime)
{
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + FriendPos.x), float(tileMapDispl.y + FriendPos.y)));
}

void Friend::render()
{
	if (!pickedUp)
		sprite->render();
}

pair <glm::ivec2, glm::ivec2> Friend::getBoundingBox() {
	glm::ivec2 x = glm::ivec2(FriendPos.x, FriendPos.x + FriendSize.x);
	glm::ivec2 y = glm::ivec2(FriendPos.y, FriendPos.y + FriendSize.y);
	return make_pair(x, y);
}

void Friend::setPosition(const glm::ivec2 initPos) {
	FriendPos = initPos;
}

void Friend::setPickedUp() {
	pickedUp = true;
}

bool Friend::isPickedUp() {
	return pickedUp;
}