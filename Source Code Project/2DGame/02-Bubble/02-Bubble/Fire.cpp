#include "Fire.h"

enum fireAnims
{
	APPEARING, MORE_FLAME
};

enum States
{
	IDLE, APPEAR, HIGH_FLAME
};





void Fire::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {

	state = IDLE;
	visible = false;
	spritesheet.loadFromFile("images/fire.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	fireSize = glm::ivec2(20.f, 80.f);
	sprite = Sprite::createSprite(glm::ivec2(20.f, 80.f), glm::vec2(0.25f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(APPEARING, 8);
	sprite->addKeyframe(APPEARING, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(APPEARING, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(MORE_FLAME, 8);
	sprite->addKeyframe(MORE_FLAME, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(MORE_FLAME, glm::vec2(0.75f, 0.f));

	delayTime = 1000.f;
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFire.x), float(tileMapDispl.y + posFire.y)));
}

void Fire::update(int deltaTime) {
	if (delayTime > 0) {
		delayTime -= deltaTime;
	}
	sprite->update(deltaTime);
	switch (state)
	{
	case IDLE:
		if (delayTime < 0) {
			if (playerNear) {
				state = APPEAR;
				visible = true;
				delayTime = 500.f;
				damagedPlayer = false;
			}
		}
		break;
	case APPEAR:
		if (sprite->animation() != APPEARING)
			sprite->changeAnimation(APPEARING);
		if (delayTime < 0) {
			state = HIGH_FLAME;
			delayTime = 500.f;
		}
		break;
	case HIGH_FLAME:

		if (sprite->animation() != MORE_FLAME)
			sprite->changeAnimation(MORE_FLAME);
		if (delayTime < 0) {
			state = IDLE;
			visible = false;
			playerNear = false;
			delayTime = 1000.f;
		}
		break;
	default:
		break;

	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFire.x), float(tileMapDispl.y + posFire.y)));
}

void Fire::render() {
	if ( visible)
		sprite->render();
}

glm::ivec2 Fire::getPosition() {
	return posFire;
}

glm::ivec2 Fire::getEnemySize() {
	return fireSize;
}

bool Fire::playerIsDmaged() {
	return damagedPlayer;
}

void Fire::setDamagedPlayer() {
	damagedPlayer = true;
}


void Fire::setTileMap(TileMap* tileMap) {
	map = tileMap;
}
void Fire::setPosition(const glm::vec2& pos) {
	posFire = pos;
}

pair<glm::ivec2, glm::ivec2> Fire::getBoundingBox() {
	glm::ivec2 x = glm::ivec2(posFire.x, posFire.x + fireSize.x);
	glm::ivec2 y = glm::ivec2(posFire.y, posFire.y + fireSize.y);
	return make_pair(x, y);
}

bool Fire::getVisible() {
	return visible;
}

void Fire::setPlayerNear() {
	playerNear = true;
}

glm::ivec2 Fire::getBase() {
	return glm::ivec2(posFire.x, posFire.y + 40 );
}

bool Fire::isNear() {
	return playerNear;
}

int Fire::getState() {
	return state;
}