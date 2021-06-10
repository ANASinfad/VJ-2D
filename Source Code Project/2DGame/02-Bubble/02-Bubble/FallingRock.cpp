#include "FallingRock.h"


#define FALL_STEP 4

enum RockAnims
{
	FALLING, CRASH,HIT_PLAYER
};

enum States
{
	IDLE, FALL, CRASHED, HIT
};

void FallingRock::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {

	damagedPlayer = false;
	playerDown = false;
	crashed = false;
	state = IDLE;
	rockSize = glm::ivec2(40.f, 20.f);
	spritesheet.loadFromFile("images/rock_tiles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(40.f, 20.f), glm::vec2(1.f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);


	sprite->setAnimationSpeed(FALLING, 8);
	sprite->addKeyframe(FALLING, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(CRASH, 8);
	sprite->addKeyframe(CRASH, glm::vec2(0.f, 0.25f));

	sprite->setAnimationSpeed(HIT_PLAYER, 8);
	sprite->addKeyframe(HIT_PLAYER, glm::vec2(0.f, 0.5f));


	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posRock.x), float(tileMapDispl.y + posRock.y)));

	delayTime = 0.f;

	
}

bool FallingRock::update(int deltaTime, glm::ivec2 playerPos) {

	checkIfPlayerIsUnderRock(playerPos);
	bool deleted = false;

	if (delayTime > 0) {
		delayTime -= deltaTime;
	}
	sprite->update(deltaTime);

	switch (state)
	{
	case IDLE:
		if (playerDown) {

			state = FALL;
		}
		break;
	case FALL:
		posRock.y += FALL_STEP;
		if (sprite->animation() != FALLING)
			sprite->changeAnimation(FALLING);
		if (damagedPlayer) {
			posRock.y -= FALL_STEP;
			state = HIT;
			delayTime = 500.f;
		}
		else if (crashed) {
			state = CRASHED;
			delayTime = 500.f;
		}
		break;
	case HIT:
		if (sprite->animation() != HIT_PLAYER)
			sprite->changeAnimation(HIT_PLAYER);
		if (delayTime < 0) {
			deleted = true;

		}
		break;
	case CRASHED:
		if (sprite -> animation() != CRASH)
			sprite->changeAnimation(CRASH);
		if (delayTime < 0) {
			deleted = true;

		}

		break;

	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posRock.x), float(tileMapDispl.y + posRock.y)));
	if (deleted) { 
		
		delete this; 
	}
	return deleted;
}


void FallingRock::render() {
		sprite->render();
}



void FallingRock::setTileMap(TileMap* tileMap) {
	map = tileMap;
}


void FallingRock::setPosition(const glm::vec2& pos) {
	posRock = pos;

}

void FallingRock::setPlayerDown() {
	playerDown = true;
}

glm::ivec2 FallingRock::getPosition() {
	return posRock;
}

glm::ivec2 FallingRock::getEnemySize(){
	return rockSize;
}

bool FallingRock::playerIsDmaged() {
	return damagedPlayer;
}

void FallingRock::setDamagedPlayer() {
	damagedPlayer = true;
}

void FallingRock::setCrashed() {
	crashed = true;
}

bool FallingRock::getCrashed() {
	return crashed;
}



pair <glm::ivec2, glm::ivec2> FallingRock::getBoundingBox() {
	glm::ivec2 x = glm::ivec2(posRock.x, posRock.x + rockSize.x);
	glm::ivec2 y = glm::ivec2(posRock.y, posRock.y + rockSize.y);
	return make_pair(x, y);
}


void FallingRock::checkIfPlayerIsUnderRock(glm::ivec2 playerPos) {
	int tileSize = map->getTileSize();
	int px = playerPos.x / tileSize;
	int py = playerPos.y / tileSize;
	int rx = (posRock.x / tileSize) - 1;
	int ry0 = (posRock.y) / tileSize;
	int tile = map->getNumTile(rx, ry0);
	bool stop = false;
	while (tile != 33 && tile != 32 && tile != 152 && !stop) {
		if (px == rx && py == ry0) {
			playerDown = true;
			stop = true;
		}
		else {
			ry0++;
		}
		tile = map->getNumTile(rx, ry0);
	}
}
