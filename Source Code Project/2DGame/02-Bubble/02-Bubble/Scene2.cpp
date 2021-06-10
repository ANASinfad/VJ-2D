#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene2.h"
#include "Game.h"

#define SCREEN_X 0
#define SCREEN_Y 40

Scene2::Scene2(glm::ivec2 posPlayer)
{
	map = NULL;
	player = NULL;
	playerPosition = posPlayer;
}

Scene2::~Scene2()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
}


void Scene2::init()
{
	initShaders();
	delayTime = 250.f;
	cameraPosition = glm::vec2(0, 0);
	map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map->setTexProgram(texProgram);
	vector<pair<glm::ivec2, int>> enemiesTiles;
	vector< glm::ivec2> posPortals;
	map->getPortalsInScene(posPortals);
	map->getEnemiesInScene(enemiesTiles, cameraPosition);
	initEnemies(enemiesTiles);
	initPortals(posPortals);
	initPlayer();
	projection = glm::ortho(cameraPosition.x, cameraPosition.x + SCREEN_WIDTH - 1, cameraPosition.y + SCREEN_HEIGHT - 1, cameraPosition.y);
	currentTime = 0.0f;
}

void Scene2::initEnemies(vector<pair<glm::ivec2, int>>& enemiesTiles) {
	enemies.clear();
	Wenemies.clear();
	waterObstacles.clear();
	rockObstacles.clear();
	fireObstacles.clear();
	for (int i = 0; i < enemiesTiles.size(); ++i) {
		if (enemiesTiles[i].second == 129) {
			Enemy* skullEnemy = new Enemy();
			skullEnemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			skullEnemy->setPosition(glm::vec2(enemiesTiles[i].first.x * map->getTileSize(), enemiesTiles[i].first.y * map->getTileSize()));
			skullEnemy->setTileMap(map);
			enemies.push_back(skullEnemy);
		}
		else if (enemiesTiles[i].second == 130) {
			WalkingSkull* walkingSkull = new WalkingSkull();
			walkingSkull->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			walkingSkull->setPosition(glm::vec2(enemiesTiles[i].first.x * map->getTileSize(), enemiesTiles[i].first.y * map->getTileSize() - 10));
			walkingSkull->setTileMap(map);
			Wenemies.push_back(walkingSkull);
		}
		else if (enemiesTiles[i].second == 131) {
			WaterDrop* water = new WaterDrop();
			water->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			water->setPosition(glm::vec2(enemiesTiles[i].first.x * map->getTileSize(), enemiesTiles[i].first.y * map->getTileSize()));
			water->setTileMap(map);
			waterObstacles.push_back(water);
		}
		else if (enemiesTiles[i].second == 132) {
			FallingRock* rock = new FallingRock();
			rock->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			rock->setPosition(glm::vec2(enemiesTiles[i].first.x * map->getTileSize(), enemiesTiles[i].first.y * map->getTileSize()));
			rock->setTileMap(map);
			rockObstacles.push_back(rock);
		}
		else if (enemiesTiles[i].second == 133) {
			Fire *fire = new Fire();
			fire->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			fire->setPosition(glm::vec2(enemiesTiles[i].first.x * map->getTileSize(), enemiesTiles[i].first.y * map->getTileSize()));
			fire->setTileMap(map);
			fireObstacles.push_back(fire);
		}
	}
}

//tantos inserts como portales en el nivel
void Scene2::initPortals(vector< glm::ivec2>& posPortals) {
	Portal port(posPortals[0], glm::ivec2(110, 2), 1);
	Portal port2(posPortals[1], glm::ivec2(22, 4), 4);
	portals.push_back(port);
	portals.push_back(port2);
}



void Scene2::initPlayer() {
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(playerPosition.x * map->getTileSize(), playerPosition.y * map->getTileSize()));
	player->setTileMap(map);
}



void Scene2::update(int deltaTime)
{
	if (player->isDead()) {
		Game::instance().changeScene(7, glm::ivec2(0, 0), player->getState());
	}
	else {
		currentTime += deltaTime;
		bool sceneChanged = false;
		updateCamera();
		updatePlayer(deltaTime, texProgram, sceneChanged);

		if (!sceneChanged && !player->hasClock()) {
			checkIFplayerNextToFire();
			checkIfDamagedByFire();
			updateEnemies(deltaTime, player->getPosition());
			checkIfRockDamaged(deltaTime);
			checkIfRockHitTheGround(deltaTime);
			checkKilledEnemies();
			checkIfatackedByEnemy();
			checkIfWatterDropDamaged();
		}
	}
}

void Scene2::updateEnemies(int deltaTime, glm::ivec2 playerPosition) {
	list<Enemy*>::iterator it = enemies.begin();
	while (it != enemies.end()) {
		(*it)->update(deltaTime, playerPosition, texProgram, cameraPosition);
		++it;
	}
	list<WalkingSkull*>::iterator it1 = Wenemies.begin();
	while (it1 != Wenemies.end()) {
		(*it1)->update(deltaTime, playerPosition, texProgram, cameraPosition);
		++it1;
	}
	list<WaterDrop*>::iterator it2 = waterObstacles.begin();
	while (it2 != waterObstacles.end()) {
		(*it2)->update(deltaTime);
		++it2;
	}
	list<FallingRock*>::iterator it3 = rockObstacles.begin();
	while (it3 != rockObstacles.end()) {
		if ((*it3)->update(deltaTime, playerPosition)) {
			it3 = rockObstacles.erase(it3);
		}
		else ++it3;
	}
	list<Fire*>::iterator it4 = fireObstacles.begin();
	while (it4 != fireObstacles.end()) {
		(*it4)->update(deltaTime);
		++it4;
	}
}



void Scene2::updatePlayer(int deltaTime, ShaderProgram& shaderProgram, bool& sceneChanged) {
	player->update(deltaTime, shaderProgram, sceneChanged, portals);
}


void Scene2::render()
{
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
	renderEnemies();
}

void Scene2::renderEnemies() {
	list<Enemy*>::iterator it = enemies.begin();
	while (it != enemies.end()) {
		(*it)->render();
		++it;
	}

	list<WalkingSkull*>::iterator it1 = Wenemies.begin();
	while (it1 != Wenemies.end()) {
		(*it1)->render();
		++it1;
	}

	list<WaterDrop*>::iterator it2 = waterObstacles.begin();
	while (it2 != waterObstacles.end()) {
		(*it2)->render();
		++it2;
	}

	list<FallingRock*>::iterator it3 = rockObstacles.begin();
	for (it3; it3 != rockObstacles.end(); it3++) {
		(*it3)->render();
	}

	list<Fire*>::iterator it4 = fireObstacles.begin();
	while (it4 != fireObstacles.end()) {
		(*it4)->render();
		++it4;
	}
}

void Scene2::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene2::updateCamera() {
	glm::ivec2 pp = player->getPosition();
	if (pp.x > cameraPosition.x + SCREEN_WIDTH) {
		cameraPosition.x += LEVEL_SIZE_X;
		refreshObjects();
	}
	else if (pp.x < cameraPosition.x) {
		cameraPosition.x -= LEVEL_SIZE_X;
		refreshObjects();
	}

	if (pp.y + player->getPlayerSize().y > cameraPosition.y + LEVEL_SIZE_Y) {
		cameraPosition.y += LEVEL_SIZE_Y;
		refreshObjects();
		glm::ivec2 pos = player->getPosition();
		pos.y += player->getPlayerSize().y;
		player->setPosition(pos);
	}
	else if (pp.y < cameraPosition.y) {
		cameraPosition.y -= LEVEL_SIZE_Y; vector<pair<glm::ivec2, int>> enemiesTiles;
		refreshObjects();
		glm::ivec2 pos = player->getPosition();
		pos.y -= player->getPlayerSize().y;
		player->setPosition(pos);
	}
	projection = glm::ortho(cameraPosition.x, cameraPosition.x + SCREEN_WIDTH - 1, cameraPosition.y + SCREEN_HEIGHT - 1, cameraPosition.y);
}


void Scene2::refreshObjects() {
	clearEnemies();
	vector<pair<glm::ivec2, int>> enemiesTiles;
	map->getEnemiesInScene(enemiesTiles, cameraPosition);
	initEnemies(enemiesTiles);
}

void Scene2::checkIfatackedByEnemy() {

	pair <glm::ivec2, glm::ivec2> playerBox = player->getBoundingBox();
	list<Enemy*>::iterator it = enemies.begin();
	for (it; it != enemies.end(); ++it) {
		pair <glm::ivec2, glm::ivec2> EnemyBox = (*it)->getBoundingBox();
		if (checkBoxOverlapping(EnemyBox, playerBox) && !player->playerIsDamaged() && !player->isUltimate()) {
			player->activateHurtTile(true);
		}
	}
	list<WalkingSkull*>::iterator it1 = Wenemies.begin();
	for (it1; it1 != Wenemies.end(); ++it1) {
		pair <glm::ivec2, glm::ivec2> EnemyBox = (*it1)->getBoundingBox();
		if (checkBoxOverlapping(EnemyBox, playerBox) && !player->playerIsDamaged() && !player->isUltimate()) {
			player->activateHurtTile(true);
		}
		if ((*it1)->hasShot()) {
			pair <glm::ivec2, glm::ivec2> shotBox = (*it1)->getShotBoundingBox();
			if (checkBoxOverlapping(shotBox, playerBox) && !player->playerIsDamaged()) {
				(*it1)->setShotImpact();
				if (!player->hasShield() && !player->isUltimate()) player->activateHurtTile(true);

			}
		}
	}
}


bool Scene2::checkBoxOverlapping(const pair<glm::ivec2, glm::ivec2>& box1, const pair<glm::ivec2, glm::ivec2>& box2) {
	bool condition1, condition2;
	condition1 = isOverlapping1D(box1.first, box2.first);
	condition2 = isOverlapping1D(box1.second, box2.second);
	return condition1 && condition2;
}


bool Scene2::isOverlapping1D(glm::ivec2 box1, glm::ivec2 box2) {
	return (box1.y >= box2.x && box2.y >= box1.x);
}


void Scene2::checkPickedUpItems() {
	pair<glm::ivec2, glm::ivec2> boundingBox1 = player->getBoundingBox();
	pair<glm::ivec2, glm::ivec2> boundingBox2 = friendItem->getBoundingBox();
	if (checkBoxOverlapping(boundingBox1, boundingBox2)) {
		if (!friendItem->isPickedUp()) {
			friendItem->setPickedUp();
			player->increaseRescuedFriend();
		}
	}
}

void Scene2::checkIfRockDamaged(int deltaTime) {

	pair<glm::ivec2, glm::ivec2> boundingBox1 = player->getBoundingBox();
	list<FallingRock*>::iterator it = rockObstacles.begin();
	for (it; it != rockObstacles.end(); ++it) {
		pair<glm::ivec2, glm::ivec2> boundingBox2 = (*it)->getBoundingBox();
		if (checkBoxOverlapping(boundingBox1, boundingBox2)) {
			if (!(*it)->playerIsDmaged() && !player->playerIsDamaged()) {
				(*it)->setDamagedPlayer();
				if (!player->hasHelmet())player->activateHurtTile(true);
			}
		}
	}
}


void Scene2::checkIfWatterDropDamaged() {
	pair<glm::ivec2, glm::ivec2> boundingBox1 = player->getBoundingBox();
	list<WaterDrop*>::iterator it = waterObstacles.begin();
	for (it; it != waterObstacles.end(); ++it) {
		pair<glm::ivec2, glm::ivec2> boundingBox2 = (*it)->getBoundingBox();
		if (checkBoxOverlapping(boundingBox1, boundingBox2)) {
			if (!(*it)->playerIsDmaged() && !player->playerIsDamaged()) {
				(*it)->setDamagedPlayer();
				if (!player->hasWaterProof())player->activateHurtTile(true);
			}
		}
	}
}

void Scene2::checkIfRockHitTheGround(int deltaTime) {
	list<FallingRock*>::iterator it = rockObstacles.begin();
	for (it; it != rockObstacles.end(); ++it) {
		if (map->elementHitsTheGround((*it)->getPosition(), glm::ivec2(16.f, 8.f))) {
			(*it)->setCrashed();
		}
	}
}

void Scene2::checkIFplayerNextToFire() {

	int tileSize = map->getTileSize();
	glm::ivec2 posPlayer = player->getPosition();
	pair<glm::ivec2, glm::ivec2> boundingBox1 = player->getBoundingBox();
	list<Fire*>::iterator it = fireObstacles.begin();
	glm::ivec2 centroid = player->centroid() / tileSize;
	int x[] = { -2,-2,0,2,2 };
	int y[] = { 0,1,1,1,0 };
	for (it; it != fireObstacles.end(); ++it) {
		pair<glm::ivec2, glm::ivec2> boundingBox2 = (*it)->getBoundingBox();
		glm::ivec2 fireSize = (*it)->getEnemySize();
		glm::ivec2 firePos = (*it)->getPosition();
		glm::ivec2 fireBase = (*it)->getBase() / tileSize;
		for (int i = 0; i < 5; ++i) {
			if (fireBase.x + x[i] == centroid.x && fireBase.y + y[i] == centroid.y && !(*it)->isNear()) {
				(*it)->setPlayerNear();
			}
		}
	}
}

void Scene2::checkIfDamagedByFire() {
	pair<glm::ivec2, glm::ivec2> boundingBox1 = player->getBoundingBox();
	list<Fire*>::iterator it = fireObstacles.begin();
	for (it; it != fireObstacles.end(); ++it) {
		pair<glm::ivec2, glm::ivec2> boundingBox2 = (*it)->getBoundingBox();
		if (checkBoxOverlapping(boundingBox1, boundingBox2) && (*it)->getState() == 2) {
			if (!(*it)->playerIsDmaged()) {
				(*it)->setDamagedPlayer();
				player->activateHurtTile(true);
			}
		}
	}
}

void Scene2::checkKilledEnemies() {
	int tileSize = map->getTileSize();
	bool leftDir = player->getDirection();
	pair <glm::ivec2, glm::ivec2> playerBox = player->getBoundingBox();
	int playerX = playerBox.first.x;
	list<Enemy*>::iterator it = enemies.begin();
	while (it != enemies.end()) {
		pair <glm::ivec2, glm::ivec2> EnemyBox = (*it)->getBoundingBox();
		int enemeyX = EnemyBox.first.x;
		if (leftDir) {
			if (checkBoxOverlapping(EnemyBox, playerBox) && (player->hitting() || player->isUltimate()) && playerX - enemeyX > 0 && !player->playerIsDamaged()) {
				delete (*it);
				*it = nullptr;
				it = enemies.erase(it);
				player->increaseExperienceAndScore(2, 100);
			}
			else ++it;
		}
		else {
			if (checkBoxOverlapping(EnemyBox, playerBox) && (player->hitting() || player->isUltimate()) && playerX - enemeyX < 0 && !player->playerIsDamaged()) {
				delete (*it);
				*it = nullptr;
				it = enemies.erase(it);
				player->increaseExperienceAndScore(2, 100);
			}
			else ++it;
		}
	}

	list<WalkingSkull*>::iterator it1 = Wenemies.begin();
	while (it1 != Wenemies.end()) {
		pair <glm::ivec2, glm::ivec2> EnemyBox = (*it1)->getBoundingBox();
		int enemeyX = EnemyBox.first.x;
		if (leftDir) {
			if (checkBoxOverlapping(EnemyBox, playerBox) && (player->hitting() || player->isUltimate()) && playerX - enemeyX > 0 && !player->playerIsDamaged()) {
				delete (*it1);
				*it1 = nullptr;
				it1 = Wenemies.erase(it1);
				player->increaseExperienceAndScore(4, 200);
			}
			else ++it1;
		}
		else {
			if (checkBoxOverlapping(EnemyBox, playerBox) && (player->hitting() || player->isUltimate()) && playerX - enemeyX < 0 && !player->playerIsDamaged()) {
				delete (*it1);
				*it1 = nullptr;
				it1 = Wenemies.erase(it1);
				player->increaseExperienceAndScore(4, 200);
			}
			else ++it1;
		}
	}
}


void Scene2::clearEnemies() {
	list<Enemy*>::iterator it = enemies.begin();
	for (it; it != enemies.end(); it++) {
		delete (*it);
		*it = nullptr;
	}
	list<WalkingSkull*>::iterator it1 = Wenemies.begin();
	for (it1; it1 != Wenemies.end(); it1++) {
		delete (*it1);
		*it1 = nullptr;
	}

	list<WaterDrop*>::iterator it2 = waterObstacles.begin();
	for (it2; it2 != waterObstacles.end(); it2++) {
		delete (*it2);
		*it2 = nullptr;
	}

	list<FallingRock*>::iterator it3 = rockObstacles.begin();
	for (it3; it3 != rockObstacles.end(); it3++) {
		delete (*it3);
		*it3 = nullptr;
	}

	list<Fire*>::iterator it4 = fireObstacles.begin();
	for (it4; it4 != fireObstacles.end(); ++it4) {
		delete (*it4);
		*it4 = nullptr;
	}
}

void Scene2::setPlayerState(PlayerState* playerState) {
	player->setState(playerState);
}

void Scene2::setPlayerPosition(glm::ivec2 pposition) {
	player->setPosition(pposition * map->getTileSize());
}