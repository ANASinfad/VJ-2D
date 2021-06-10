#pragma once

#include <glm/glm.hpp>
#include <iostream>
class Portal
{
public:

	Portal(glm::ivec2 pos, glm::ivec2 des, int level);
	glm::ivec2 getPosition();
	glm::ivec2 getDesitination();
	int getLevel();


private:
	glm::ivec2 position;
	glm::ivec2 destination;
	int level;

};

