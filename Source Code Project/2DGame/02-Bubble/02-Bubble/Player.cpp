#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 3
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define CLIMB_STEP 1
#define ULTI_STEP 5




enum PlayerMovementDirection {
	LEFT_D, RIGHT_D
};

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_RIGHT, JUMP_LEFT, DOWN_RIGHT, DOWN_LEFT, CLIMB, STAND_ROPE, DAMAGED_RIGHT, DAMAGED_LEFT, ATTACK_RIGHT, ATTACK_LEFT, LOAD_ULTI_RIGHT, LOAD_ULTI_LEFT, LOAD_ULTI_RIGHT_2, LOAD_ULTI_LEFT_2, ULTI_ANIM
};

enum States
{
	STAND, WALK, JUMP, DIVING, CLIMBING, DAMAGED, ATTACK, PREPARING_ULTI, ULTI
};




void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	currentTime = 0;
	ultiTimer = 0;
	speedStep = 2;
	state = STAND;
	direction = LEFT_D;
	bJumping = false;
	bAttack = false;
	bClimb = false;
	key = false;
	isDamaged = false;
	dead = false;
	godMode = false;
	sideMargins = glm::ivec2(10, 15);
	playerSize = glm::ivec2(50, 50);
	delayTime = 0.0;
	spritesheet.loadFromFile("images/SonicPlayer.png", TEXTURE_PIXEL_FORMAT_RGBA);
	// 32 * 32 es el tamaño del tile
	// nos movemos con 0.2f
	sprite = Sprite::createSprite(playerSize, glm::vec2(0.2f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(19);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.2f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(JUMP_RIGHT, 8);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.f, 0.3f));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.2f, 0.3f));

	sprite->setAnimationSpeed(DOWN_RIGHT, 8);
	sprite->addKeyframe(DOWN_RIGHT, glm::vec2(0.4f, 0.3f));

	sprite->setAnimationSpeed(JUMP_LEFT, 8);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.f, 0.4f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.2f, 0.4f));

	sprite->setAnimationSpeed(DOWN_LEFT, 8);
	sprite->addKeyframe(DOWN_LEFT, glm::vec2(0.4f, 0.4f));

	sprite->setAnimationSpeed(MOVE_LEFT, 10);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f, 0.2f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.2f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4f, 0.2f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.6f, 0.2f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.8f, 0.2f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 10);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.1f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.1f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.1f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.6f, 0.1f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.8f, 0.1f));

	sprite->setAnimationSpeed(CLIMB, 5);
	sprite->addKeyframe(CLIMB, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(CLIMB, glm::vec2(0.2f, 0.5f));
	sprite->addKeyframe(CLIMB, glm::vec2(0.4f, 0.5f));
	sprite->addKeyframe(CLIMB, glm::vec2(0.2f, 0.5f));

	sprite->setAnimationSpeed(STAND_ROPE, 5);
	sprite->addKeyframe(STAND_ROPE, glm::vec2(0.2f, 0.5f));

	sprite->setAnimationSpeed(DAMAGED_RIGHT, 5);
	sprite->addKeyframe(DAMAGED_RIGHT, glm::vec2(0.0f, 0.6f));


	sprite->setAnimationSpeed(DAMAGED_LEFT, 5);
	sprite->addKeyframe(DAMAGED_LEFT, glm::vec2(0.2f, 0.6f));

	sprite->setAnimationSpeed(ATTACK_RIGHT, 5);
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.4f, 0.f));

	sprite->setAnimationSpeed(ATTACK_LEFT, 5);
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.6f, 0.f));

	sprite->setAnimationSpeed(LOAD_ULTI_RIGHT, 20);
	sprite->addKeyframe(LOAD_ULTI_RIGHT, glm::vec2(0.f, 0.7f));
	sprite->addKeyframe(LOAD_ULTI_RIGHT, glm::vec2(0.2f, 0.7f));

	sprite->setAnimationSpeed(LOAD_ULTI_RIGHT_2, 5);
	sprite->addKeyframe(LOAD_ULTI_RIGHT_2, glm::vec2(0.4f, 0.7f));
	sprite->addKeyframe(LOAD_ULTI_RIGHT_2, glm::vec2(0.6f, 0.7f));
	sprite->addKeyframe(LOAD_ULTI_RIGHT_2, glm::vec2(0.8f, 0.7f));

	sprite->setAnimationSpeed(ULTI_ANIM, 15);
	sprite->addKeyframe(ULTI_ANIM, glm::vec2(0.f, 0.9f));
	sprite->addKeyframe(ULTI_ANIM, glm::vec2(0.2f, 0.9f));

	sprite->setAnimationSpeed(LOAD_ULTI_LEFT, 20);
	sprite->addKeyframe(LOAD_ULTI_LEFT, glm::vec2(0.f, 0.8f));
	sprite->addKeyframe(LOAD_ULTI_LEFT, glm::vec2(0.2f, 0.8f));

	sprite->setAnimationSpeed(LOAD_ULTI_LEFT_2, 5);
	sprite->addKeyframe(LOAD_ULTI_LEFT_2, glm::vec2(0.4f, 0.8f));
	sprite->addKeyframe(LOAD_ULTI_LEFT_2, glm::vec2(0.6f, 0.8f));
	sprite->addKeyframe(LOAD_ULTI_LEFT_2, glm::vec2(0.8f, 0.8f));


	sprite->changeAnimation(STAND_LEFT);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	playerState = new PlayerState();
	playerState->init(glm::ivec2(0, 0), glm::ivec2(0, 0), shaderProgram);
}

void Player::update(int deltaTime, ShaderProgram& shaderProgram, bool& sceneChanged, vector<Portal>& portals)
{
	currentTime += deltaTime;
	cheats();
	checkForItems();
	if (hasClock() && currentTime - clockTimer > 5000) {
		playerState->clearClock();
		powerUpsObtained[4] = false;
	}

	if (delayTime > 0) {
		delayTime -= deltaTime;
	}
	sprite->update(deltaTime);
	bool leftB = Game::instance().getSpecialKey(GLUT_KEY_LEFT);
	bool rightB = Game::instance().getSpecialKey(GLUT_KEY_RIGHT);
	bool upB = Game::instance().getSpecialKey(GLUT_KEY_UP);
	bool special = Game::instance().getKey('z');
	bool downB = Game::instance().getSpecialKey(GLUT_KEY_DOWN);
	bAttack = Game::instance().getKey(' ');
	updatePlayerDirection(leftB, rightB);

	switch (state)
	{
	case STAND:
		//state treatment
		int tileAdjust;
		if (delayTime <= 0) {
			if ((leftB && direction == LEFT_D && !map->collisionMoveLeft(posPlayer, playerSize, &posPlayer.x, sideMargins)) || (rightB && direction == RIGHT_D && !map->collisionMoveRight(posPlayer, playerSize, &posPlayer.x, sideMargins)))
				state = WALK;
			else if (upB && !bJumping && map->teleportTile(centroid(), glm::ivec2(32.f, 32.f))) {
				//setPosition(glm::ivec2(10 * map->getTileSize(),5 * map->getTileSize()));
				upB = false;
				sceneChanged = true;
				powerUpsObtained[4] = false;
				playerState->clearClock();
				treatPortals(portals);
			}
			else if (!bJumping && upB  ) {
				engine = SoundManager::instance().getSoundEngine();
				engine->play2D("sounds/jump.wav");
				state = JUMP;
			}
			else if (!map->collisionMoveDown(posPlayer, playerSize, &posPlayer.y, sideMargins))
				state = DIVING;
			else if (bAttack) {
				state = ATTACK;
				delayTime = 175.f;
			}
			if (special && powerUpsObtained[3]) {
				state = PREPARING_ULTI;
				ultiTimer = currentTime;
				ultiDirection = direction;
			}

			if (isDamaged) {
				engine = SoundManager::instance().getSoundEngine();
				engine->play2D("sounds/damaged.wav");
				state = DAMAGED;
				delayTime = 500.f;
			}


			if (downB && map->ropeDownstairs(posPlayer, playerSize, &tileAdjust)) {
				bClimb = true;
				state = CLIMBING;
			}
			else if (upB && map->ropeUpstairs(posPlayer, playerSize, &tileAdjust)) {
				bClimb = true;
				state = CLIMBING;
			}

			//treat behaviour
			if (!bJumping && upB && !bClimb) {
				jumpAngle = 0;
				bJumping = true;
				startY = posPlayer.y;
			}
			else if (bClimb) {
				if (map->ropeDownstairs(posPlayer, playerSize, &tileAdjust))
					posPlayer.y += 22;
				else
					posPlayer.y -= 5;
				map->adjustPosition(posPlayer, playerSize, sideMargins, tileAdjust);
			}

		}





		break;
	case WALK:
		//state treatment
		if (!leftB && !rightB)
			state = STAND;
		else if (upB)
			state = JUMP;
		else if (!map->collisionMoveDown(posPlayer, playerSize, &posPlayer.y, sideMargins))
			state = DIVING;
		else if (bAttack) {
			delayTime = 150.f;
			state = ATTACK;
		}
		if (isDamaged) {
			delayTime = 500.f;
			state = DAMAGED;
			engine = SoundManager::instance().getSoundEngine();
			engine->play2D("sounds/damaged.wav");
		}

		//behaviour treatment
		else if (direction == LEFT_D) {
			posPlayer.x -= speedStep;
			if (map->collisionMoveLeft(posPlayer, playerSize, &posPlayer.x, sideMargins)) //poner padding aquí
			{
				//posPlayer.x += SPEED_STEP;
				state = STAND;
			}
		}
		else {
			posPlayer.x += speedStep;
			if (map->collisionMoveRight(posPlayer, playerSize, &posPlayer.x, sideMargins))//poner padding aquí
			{
				//posPlayer.x -= SPEED_STEP;
				state = STAND;
			}
		}
		break;
	case JUMP:
		//state treatment
		if (!bJumping && map->collisionMoveDown(posPlayer, playerSize, &posPlayer.y, sideMargins))
			state = STAND;
		else if (!bJumping)
			state = DIVING;
		else if (jumpAngle == 180)
			state = DIVING;
		else if (map->collisionMoveUp(posPlayer, playerSize, &posPlayer.y, sideMargins)) {
			state = DIVING;
			bJumping = false;
		}

		//behaviour treatment
		if (bJumping) {
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else
			{
				posPlayer.y = int(startY - 40 * sin(3.14159f * jumpAngle / 180.f));
				bJumping = !map->collisionMoveDown(posPlayer, playerSize, &posPlayer.y, sideMargins);
			}

			if (leftB) {
				posPlayer.x -= speedStep;
				if (map->collisionMoveLeft(posPlayer, playerSize, &posPlayer.x, sideMargins)) //poner padding aquí
					posPlayer.x += speedStep;
			}
			else if (rightB) {
				posPlayer.x += speedStep;
				if (map->collisionMoveRight(posPlayer, playerSize, &posPlayer.x, sideMargins))//poner padding aquí
					posPlayer.x -= speedStep;
			}
		}

		//graphics treatment
		break;
	case DIVING:
		//behaviour treatment
		posPlayer.y += FALL_STEP;
		if (leftB) {
			posPlayer.x -= speedStep;
			if (map->collisionMoveLeft(posPlayer, playerSize, &posPlayer.x, sideMargins)) //poner padding aquí
				posPlayer.x += speedStep;
		}
		else if (rightB) {
			posPlayer.x += speedStep;
			if (map->collisionMoveRight(posPlayer, playerSize, &posPlayer.x, sideMargins))//poner padding aquí
				posPlayer.x -= speedStep;
		}

		//state treatment
		if (map->collisionMoveDown(posPlayer, playerSize, &posPlayer.y, sideMargins))
			state = STAND;
		break;
	case CLIMBING:
		if (bClimb && ((map->lastRope(posPlayer, playerSize) && upB) || (map->touchTheSurface(posPlayer, playerSize) && downB))) {
			if (map->lastRope(posPlayer, playerSize)) posPlayer.y -= 25;
			delayTime = 250.f;
			bClimb = false;
			state = STAND;
		}
		else if (isDamaged) {
			bClimb = false;
			state = DIVING;
		}


		if (bClimb) {
			if (upB)
				posPlayer.y -= speedStep;
			else if (downB)
				posPlayer.y += speedStep;
		}

		break;

	case DAMAGED:
		//tratamiento del health
		if (delayTime < 0) {
			state = STAND;
			isDamaged = false;
		}
		break;
	case ATTACK:

		if (delayTime < 0) {
			engine = SoundManager::instance().getSoundEngine();
			engine->play2D("sounds/jab.wav");
			state = STAND;
			bAttack = false;
		}
		break;
	case PREPARING_ULTI:
		if ((leftB && direction == LEFT_D && !map->collisionMoveLeft(posPlayer, playerSize, &posPlayer.x, sideMargins)) || (rightB && direction == RIGHT_D && !map->collisionMoveRight(posPlayer, playerSize, &posPlayer.x, sideMargins)))
			state = WALK;
		else if (isDamaged) {
			delayTime = 500.f;
			state = DAMAGED;
		}
		else if (!bJumping && upB)
			state = JUMP;
		else if (bAttack) {
			state = ATTACK;
			delayTime = 175.f;
		}
		else if (currentTime - ultiTimer > 500) {
			state = ULTI;
		}
		break;
	case ULTI:
		bool collisionLeft = map->collisionMoveLeft(posPlayer, playerSize, &posPlayer.x, sideMargins);
		bool collisionRight = map->collisionMoveRight(posPlayer, playerSize, &posPlayer.x, sideMargins);
		int tileSize = map->getTileSize();
		if (direction == LEFT_D && !collisionLeft && ultiDirection == LEFT_D) {
			posPlayer.x -= ULTI_STEP;
		}
		else if (direction == LEFT_D) {
			direction = ultiDirection;
			state = STAND;
		}


		if (direction == RIGHT_D && !collisionRight && ultiDirection == RIGHT_D) {
			posPlayer.x += ULTI_STEP;
		}
		else if (direction == RIGHT_D) {
			direction = ultiDirection;
			state = STAND;
		}

		if (isDamaged) {
			delayTime = 500.f;
			state = DAMAGED;
		}
		else if (!map->collisionMoveDown(posPlayer, playerSize, &posPlayer.y, sideMargins))
			state = STAND;
		else if (!bJumping && upB)
			state = JUMP;
		break;
	}
	treatGraphics();
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	updateState(shaderProgram);
}

void Player::updateState(ShaderProgram& shaderProgram) {
	playerState->update(posPlayer, playerSize, 640, 400, shaderProgram);
}

void Player::updatePlayerDirection(const bool& leftB, const bool& rightB) {
	if (leftB)
		direction = LEFT_D;
	if (rightB)
		direction = RIGHT_D;
}

void Player::standGraphicsTreatment() {
	switch (direction)
	{
	case LEFT_D:

		break;
	case RIGHT_D:

		break;
	default:
		break;
	}
}

void Player::walkGraphicsTreatment() {

}

void Player::treatGraphics() {
	switch (state)
	{
	case STAND:
		if (direction == LEFT_D) {
			if (sprite->animation() != STAND_LEFT)
				sprite->changeAnimation(STAND_LEFT);
		}
		else {
			if (sprite->animation() != STAND_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
		}
		break;
	case WALK:
		if (direction == LEFT_D) {
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
		}
		else {
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
		}
		break;
	case JUMP:
		if (direction == LEFT_D) {
			if (sprite->animation() != JUMP_LEFT)
				sprite->changeAnimation(JUMP_LEFT);
			if (jumpAngle > 90)
				sprite->changeAnimation(DOWN_LEFT);
		}
		else {
			if (sprite->animation() != JUMP_RIGHT)
				sprite->changeAnimation(JUMP_RIGHT);
			if (jumpAngle > 90)
				sprite->changeAnimation(DOWN_RIGHT);
		}
		break;
	case DIVING:
		if (direction == LEFT_D) {
			if (sprite->animation() != DOWN_LEFT)
				sprite->changeAnimation(DOWN_LEFT);
		}
		else {
			if (sprite->animation() != DOWN_RIGHT)
				sprite->changeAnimation(DOWN_RIGHT);
		}
		break;
	case CLIMBING:
		if (sprite->animation() != CLIMB)
			sprite->changeAnimation(CLIMB);
		break;
	case DAMAGED:
		if (direction == LEFT_D) {
			if (sprite->animation() != DAMAGED_LEFT)
				sprite->changeAnimation(DAMAGED_LEFT);
		}
		else {
			if (sprite->animation() != DAMAGED_RIGHT)
				sprite->changeAnimation(DAMAGED_RIGHT);
		}
		break;
	case ATTACK:
		if (direction == LEFT_D) {
			if (sprite->animation() != ATTACK_LEFT)
				sprite->changeAnimation(ATTACK_LEFT);
		}
		else {
			if (sprite->animation() != ATTACK_RIGHT)
				sprite->changeAnimation(ATTACK_RIGHT);
		}
		break;
	case PREPARING_ULTI:
		if (ultiDirection == LEFT_D) {
			if (sprite->animation() != LOAD_ULTI_LEFT && currentTime - ultiTimer <= 400)
				sprite->changeAnimation(LOAD_ULTI_LEFT);
			else if (sprite->animation() != LOAD_ULTI_LEFT_2 && currentTime - ultiTimer > 400)
				sprite->changeAnimation(LOAD_ULTI_LEFT_2);
		}
		else {
			if (sprite->animation() != LOAD_ULTI_RIGHT && currentTime - ultiTimer <= 400)
				sprite->changeAnimation(LOAD_ULTI_RIGHT);
			else if (sprite->animation() != LOAD_ULTI_RIGHT_2 && currentTime - ultiTimer > 400)
				sprite->changeAnimation(LOAD_ULTI_RIGHT_2);
		}
		break;
	case ULTI:
		if (sprite->animation() != ULTI_ANIM)
			sprite->changeAnimation(ULTI_ANIM);
		break;
	default:
		break;
	}
}

void Player::render()
{
	renderState();
	sprite->render();
}

void Player::renderState() {
	playerState->render();
}

void Player::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::activateHurtTile(bool value) {
	if (!godMode) {
		isDamaged = value;
		if (playerState->increasePlayerLife(-1) == 0)
			dead = true;
	}
}
pair <glm::ivec2, glm::ivec2> Player::getBoundingBox() {
	glm::ivec2 x = glm::ivec2(posPlayer.x + sideMargins.x, posPlayer.x + playerSize.x - 1 - sideMargins.x);
	glm::ivec2 y = glm::ivec2(posPlayer.y + sideMargins.y, posPlayer.y + playerSize.y - 1 - sideMargins.y);
	return make_pair(x, y);
}

glm::ivec2 Player::centroid() {
	return glm::ivec2(posPlayer.x + playerSize.x / 2, posPlayer.y + playerSize.y / 2);

}
bool Player::playerIsDamaged() {
	return isDamaged;
}

//returns true if player is looking left
bool Player::getDirection() {
	return direction == LEFT_D;
}

bool Player::hitting() {
	return bAttack;
}

void Player::checkForItems() {
	bool updateScene = false;
	glm::ivec2 sonicPosition = centroid() / map->getTileSize();
	int tile = map->getNumTile(sonicPosition.x, sonicPosition.y);
	if (tile == 229) {//treasures
		int item = rand() % 10;
		if (item < 5 && powerUpsObtained[item]) item = 5;
		else if (item < 5) {
			powerUpsObtained[item] = true;
			if (item == 3) speedStep += 2; //hyper-shoes
			else if (item == 4) clockTimer = currentTime;
		}
		map->setTile(sonicPosition.x, sonicPosition.y, 1);
		playerState->showItem(item);
		updateScene = true;
	}
	else if (tile == 228 && !key) {//key
		key = true;
		playerState->setKey(true);
		map->setTile(sonicPosition.x, sonicPosition.y, 1);
		engine = SoundManager::instance().getSoundEngine();
		engine->play2D("sounds/key.wav");
		updateScene = true;
	}
	else if (tile == 230 && key && map->getNumTile(sonicPosition.x, sonicPosition.y + 1) != 230) {
		key = false;
		playerState->setKey(false);
		if (map->getNumTile(sonicPosition.x, sonicPosition.y - 1) == 230)
			map->setTile(sonicPosition.x, sonicPosition.y - 1, 1);
		else {
			map->setTile(sonicPosition.x, sonicPosition.y, 1);

			//aqui buscar que tile va aquÃ­ y ponerla
			int prize = map->getPrize(sonicPosition.x + 2, sonicPosition.y);
			map->setTile(sonicPosition.x + 2, sonicPosition.y, prize);

			if (prize == 261)
				map->setTile(sonicPosition.x + 2, sonicPosition.y + 1, 293);
			else
				map->setTile(sonicPosition.x + 2, sonicPosition.y + 1, 33);
			map->setTile(sonicPosition.x + 1, sonicPosition.y + 1, 70);
			map->setTile(sonicPosition.x + 3, sonicPosition.y + 1, 72);//257 258 259

			map->setTile(sonicPosition.x + 1, sonicPosition.y, 289);
			map->setTile(sonicPosition.x + 3, sonicPosition.y, 291);

			map->setTile(sonicPosition.x + 2, sonicPosition.y - 1, 258);
			map->setTile(sonicPosition.x + 1, sonicPosition.y - 1, 257);
			map->setTile(sonicPosition.x + 3, sonicPosition.y - 1, 259);
		}
		engine = SoundManager::instance().getSoundEngine();
		engine->play2D("sounds/arrow.wav");
		updateScene = true;
	}
	else if (tile == 261) {
		engine = SoundManager::instance().getSoundEngine();
		engine->play2D("sounds/friend.wav");
		increaseRescuedFriend();
		map->setTile(sonicPosition.x, sonicPosition.y, 1);
		map->setTile(sonicPosition.x, sonicPosition.y + 1, 33);
		updateScene = true;

	}
	if (updateScene) map->updateTiles();
}

void Player::cheats() {
	if (Game::instance().getKey('i')) {
		playerState->increasePlayerExperience(1);
	}
	else if (Game::instance().getKey('k')) {
		if (playerState->increasePlayerLife(-1) == 0) dead = true;
	}
	else if (Game::instance().getKey('p')) {
		playerState->increasePlayerScore(1);
	}
	else if (Game::instance().getKey('+')) {
		increaseRescuedFriend();
	}
	else if (Game::instance().getKey('g')) {
		godMode = true;
		playerState->setGodMode();
	}
	else if (Game::instance().getKey('q') && !powerUpsObtained[0]) {
		powerUpsObtained[0] = true;
		playerState->showItem(0);
	}
	else if (Game::instance().getKey('w') && !powerUpsObtained[1]) {
		powerUpsObtained[1] = true;
		playerState->showItem(1);
	}
	else if (Game::instance().getKey('e') && !powerUpsObtained[2]) {
		powerUpsObtained[2] = true;
		playerState->showItem(2);
	}
	else if (Game::instance().getKey('r') && !powerUpsObtained[3]) {
		powerUpsObtained[3] = true;
		speedStep += 2;
		playerState->showItem(3);
	}
	else if (Game::instance().getKey('t') && !powerUpsObtained[4] ) {
		powerUpsObtained[4] = true;
		clockTimer = currentTime;
		playerState->showItem(4);
	}
}

void Player::increaseRescuedFriend() {
	if (playerState->increaseRescuedFriends() >= 6) win = true;
}

void Player::increaseExperienceAndScore(int exp, int score) {
	playerState->increasePlayerExperience(exp);
	playerState->increasePlayerScore(score);
}

bool Player::hasHelmet() {
	return powerUpsObtained[0];
}

bool Player::hasWaterProof() {
	return powerUpsObtained[1];
}

bool Player::hasShield() {
	return powerUpsObtained[2];
}

bool Player::hasClock() {
	return powerUpsObtained[4];
}

void Player::treatPortals(vector<Portal>& portals) {

	//

	//set<Portal>::const_iterator it = portals.begin();
	int tileSize = map->getTileSize();
	glm::ivec2 cent = centroid() / tileSize;
	bool found = false;
	int level = 1;
	glm::ivec2 destination;
	for (int i = 0; i < portals.size() && !found; ++i) {
		if (portals[i].getPosition() == cent) {
			found = true;
			level = portals[i].getLevel();
			destination = portals[i].getDesitination();
		}
	}
	if (level == 8) {
		if (win) {
			Game::instance().changeScene(level, destination, playerState);
			engine = SoundManager::instance().getSoundEngine();
			engine->play2D("sounds/teleport.wav");
		}
	}
	else {
		Game::instance().changeScene(level, destination, playerState);
		engine = SoundManager::instance().getSoundEngine();
		engine->play2D("sounds/teleport.wav");
	}
	
}

void Player::setState(PlayerState* playerState) {
	/*if (playerState->hasClock()) {
		playerState->clearClock();
	}*/
	if (playerState->isGodMode()) godMode = true;
	if (playerState->hasKey()) key = true;
	if (playerState->hasHelmet()) powerUpsObtained[0] = true;
	if (playerState->hasWaterProof()) powerUpsObtained[1] = true;
	if (playerState->hasShield()) powerUpsObtained[2] = true;
	if (playerState->hasHypershoes()) {
		speedStep += 2;
		powerUpsObtained[3] = true;
	}
	this->playerState = playerState;
}

bool Player::isUltimate() {
	return state == ULTI;
}

bool Player::isDead() {
	return dead;
}

bool Player::hasWon() {
	return win;
}

PlayerState* Player::getState() {
	return playerState;
}