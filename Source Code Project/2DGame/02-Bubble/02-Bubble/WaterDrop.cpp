#include "WaterDrop.h"

#define FALL_STEP 2

enum waterAnims
{
	APPEARING, FALLING, CRASH, HITPLAYER
};


enum States
{
	APPEAR,FALL,CRASHED,HIT
};

void WaterDrop::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	crashed = false;
	renderTile = false;
	damagedPlayer = false;
	state = APPEAR;
	waterSize = glm::ivec2(20.f, 20.f);
	spritesheet.loadFromFile("images/water_sprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(20.f, 20.f), glm::vec2(0.2f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);
	
	sprite->setAnimationSpeed(APPEARING, 8);
	sprite->addKeyframe(APPEARING, glm::vec2(0.f, 0.f));

	sprite->addKeyframe(APPEARING, glm::vec2(0.2f, 0.f));
	

	sprite->setAnimationSpeed(FALLING, 8);
	sprite->addKeyframe(FALLING, glm::vec2(0.4f, 0.f));

	sprite->setAnimationSpeed(CRASH, 8);
	sprite->addKeyframe(CRASH, glm::vec2(0.6f, 0.f));


	sprite->setAnimationSpeed(HITPLAYER, 8);
	sprite->addKeyframe(HITPLAYER, glm::vec2(0.8f, 0.f));

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posWater.x), float(tileMapDispl.y + posWater.y)));
	
	delayTime = 2000.f;

}

void WaterDrop::update(int deltaTime) {


	if (delayTime > 0) {
		delayTime -= deltaTime;
	}
	sprite->update(deltaTime);
	switch (state)
	{
	case APPEAR:
		damagedPlayer = false;

		if (delayTime < 0) {
			if (sprite->animation() != APPEARING)
				sprite->changeAnimation(APPEARING);
			renderTile = true;
			state = FALL;
			delayTime = 200.f;		
		}
		
		
		break;
	case FALL:
		if (delayTime < 0) {
			posWater.y += FALL_STEP;
			if (sprite->animation() != FALLING)
				sprite->changeAnimation(FALLING);
			if (map->elementHitsTheGround(posWater, glm::ivec2(8.f, 8.f)) && !crashed) {
				state = CRASHED;
				delayTime = 500.f;
			}
			if (damagedPlayer) {
				state = HIT;
				delayTime = 500.f;
			}
		}
		break;
	case CRASHED:
		if (sprite->animation() != CRASH)
			sprite->changeAnimation(CRASH);
		crashed = true;
		if (delayTime < 0) {
			//desactivar la tile
			renderTile = false;
			posWater = posInit;
			crashed = false;
			delayTime = 2000.f;
			state = APPEAR;
		}
		break;
	case HIT:
		if (sprite->animation() != HITPLAYER)
			sprite->changeAnimation(HITPLAYER);
		if (delayTime < 0) {	
			damagedPlayer = false;
			renderTile = false;
			delayTime = 2000.f;
			state = APPEAR;
			posWater = posInit;
		}
		break;
	default:
		break;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posWater.x), float(tileMapDispl.y + posWater.y)));
}


void WaterDrop::render() {
	if (renderTile)
		sprite->render();
}

void WaterDrop::setTileMap(TileMap* tileMap) {
	map = tileMap;
}


void WaterDrop::setPosition(const glm::vec2& pos) {
	posWater = pos;
	posInit = posWater;
}

glm::ivec2 WaterDrop::getPosition() {
	return posWater;
}

glm::ivec2 WaterDrop::getEnemySize() {
	return waterSize;
}

pair <glm::ivec2, glm::ivec2> WaterDrop::getBoundingBox() {
	glm::ivec2 x = glm::ivec2(posWater.x, posWater.x + waterSize.x);
	glm::ivec2 y = glm::ivec2(posWater.y, posWater.y + waterSize.y);
	return make_pair(x, y);
}


bool WaterDrop::playerIsDmaged() {
	return damagedPlayer;
}

void WaterDrop::setDamagedPlayer() {
	damagedPlayer = true;
}