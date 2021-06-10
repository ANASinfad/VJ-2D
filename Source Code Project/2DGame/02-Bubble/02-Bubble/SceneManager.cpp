#include "SceneManager.h"


void SceneManager::init(PlayerState * playerState) {
	currentScene->init();
	if ( playerState != nullptr) currentScene->setPlayerState(playerState);
}

void SceneManager::update(int deltaTime) {
	currentScene->update(deltaTime);
}

void SceneManager::render() {
	currentScene->render();
}

void SceneManager::changeScene(GeneralScene* scene, glm::ivec2 pposition, bool existed) {
	if (existed) scene->setPlayerPosition(pposition);
	currentScene = scene;
}