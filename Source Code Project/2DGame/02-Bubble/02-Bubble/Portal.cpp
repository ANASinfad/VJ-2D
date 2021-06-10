#include "Portal.h"


Portal::Portal(glm::ivec2 pos, glm::ivec2 des, int lvl) {

	position = pos;
	destination = des;
	level = lvl;
}


glm::ivec2 Portal::getPosition() {
	return position;
}

glm::ivec2 Portal::getDesitination() {
	return destination;
}

int Portal::getLevel() {
	return level;
}

