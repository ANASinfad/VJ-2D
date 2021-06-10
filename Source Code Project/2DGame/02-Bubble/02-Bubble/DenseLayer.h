
#include<iostream>
#include<vector>
using namespace std;

#pragma once
struct Tile
{
	int index;
	// Atributs del tile
	// Per exemple:
	bool bWall;
};

class DenseLayer
{
protected:
	vector<Tile> map;
	int mapWidth, mapHeight;
	int tileWidth, tileHeight;
	int depth;

public:
	void render();
};

