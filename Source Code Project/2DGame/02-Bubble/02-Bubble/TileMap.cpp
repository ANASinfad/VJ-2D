#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
using namespace std;

#define FRIEND_CODE 261
#define TREASURE_CODE 229


void TileMap::setTexProgram(ShaderProgram& program) {
	shaderProgram = program;
}

TileMap* TileMap::createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program, list<glm::ivec2>& friendsInPosition)
{
	TileMap* map = new TileMap(levelFile, minCoords, program,friendsInPosition);
	return map;
}
TileMap* TileMap::createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	list<glm::ivec2> friendsInPosition;
	TileMap* map = new TileMap(levelFile, minCoords, program, friendsInPosition);
	return map;
}


TileMap::TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program, list<glm::ivec2>& friendsInPosition)
{
	loadLevel(levelFile, friendsInPosition);
	prepareArrays(minCoords, program);
	tilesToIgnore = set<int>{ 0, 1, 168, 169, 58, 203, 202 ,53, 52, 34, 136, 135, 141, 145, 146, 289, 290, 291, 257, 225, 226, 258, 259, 227,211,177,170,178,212,204,98,179,99,213,2, 229,129, 130, 133, 228, 230, 262, 263,264,293,261,2,132,131 };
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
	//renderExtras();
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile, list<glm::ivec2>& friendsInPosition)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			int currentTile = readNextTile(fin);
			if (currentTile != -1) {
				if ((currentTile == FRIEND_CODE || currentTile == TREASURE_CODE) && map[j*mapSize.x + i -1] == 262  ) {
					doorPrizes.push_back(make_pair(glm::ivec2(i, j), currentTile));
					map[j * mapSize.x + i] = 263;
				}
				else map[j * mapSize.x + i] = currentTile;
			}

			/*fin.get(tile);
			if(tile == ' ')
				map[j*mapSize.x+i] = 0;
			else
				map[j*mapSize.x+i] = tile - int('0');*/
		}
		//fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	initCoords = minCoords;
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)% tilesheetSize.x) / tilesheetSize.x, float((tile-1)/ tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, int* posX, const glm::ivec2& sideMargins) const
{
	int x, y0, y1;
	
	x = (pos.x+sideMargins.x-1) / tileSize;
	y0 = (pos.y) / tileSize;
	y1 = (pos.y + size.y - 1 - sideMargins.y) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		int tileIndex = map[y * mapSize.x + x];
		if (tilesToIgnore.find(tileIndex) == tilesToIgnore.end()) {
			if (*posX - tileSize * x + sideMargins.x > 0)
			{
				*posX = tileSize * x + tileSize - sideMargins.x;
				return true;
			}
			//else if (*posX - tileSize * x + sideMargins.x == 0) return false;
		}
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, int* posX, const glm::ivec2& sideMargins) const
{
	int x, y0, y1;
	int xprima = pos.x / tileSize;
	x = (pos.x + size.x - sideMargins.x) / tileSize;
	y0 = (pos.y) / tileSize;
	y1 = (pos.y + size.y - 1 - sideMargins.y) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		int tileIndex = map[y * mapSize.x + x];
		if (tilesToIgnore.find(tileIndex) == tilesToIgnore.end()){
			if (*posX + size.x - 1 - (tileSize * x) >= 0) {
				*posX = xprima * tileSize;
				return true;
			}
			//return true;
		}
	}
	
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, const glm::ivec2& sideMargins) const
{
	int x0, x1, y;
	
	x0 = (pos.x + sideMargins.x) / tileSize;
	x1 = (pos.x + size.x - 1 - sideMargins.x) / tileSize;
	y = (pos.y + size.y - sideMargins.y) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		int tileIndex = map[y * mapSize.x + x];
		if (tilesToIgnore.find(tileIndex) == tilesToIgnore.end())
		{
			if(*posY - tileSize * y + size.y - sideMargins.y <= 4)
			{
				*posY = tileSize * y - size.y + sideMargins.y;
				return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY, const glm::ivec2& sideMargins) const
{
	int x0, x1, y;

	x0 = (pos.x + sideMargins.x) / tileSize;
	x1 = (pos.x + size.x - 1 - sideMargins.x) / tileSize;
	y = (pos.y + sideMargins.y) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		int tileIndex = map[y * mapSize.x + x];
		if (tilesToIgnore.find(tileIndex) == tilesToIgnore.end())
		{
			if (*posY - tileSize * y + sideMargins.y >= 4)
			{
				*posY = tileSize * y + sideMargins.y;
				return true;
			}
		}
	}

	return false;
}

bool TileMap::nearRope(const glm::ivec2 posPlayer, const glm::ivec2& size) const {
	int y = (posPlayer.y + size.y) / tileSize;
	int x = (posPlayer.x + size.x/2) / tileSize;
	int yprima = y + 2;
	for (int i = y; i < yprima; ++i) {
		int tileIndex = map[i * mapSize.x + x];
		if (tileIndex == 53 || tileIndex == 52 || tileIndex == 34) return true;
	}
	return false;
}

bool TileMap::ropeUpstairs(const glm::ivec2 posPlayer, const glm::ivec2& size, int* tileAdjust) const
{
	int x, x1, y;
	x = (posPlayer.x + size.x / 2)/ tileSize;
	x1 = (posPlayer.x + size.x -1 ) / tileSize;
	y = (posPlayer.y + size.y -1 ) / tileSize;
	int tileIndex = map[y * mapSize.x + x];
	*tileAdjust = x;
	if (tileIndex == 53 || tileIndex == 52 || tileIndex == 34)   return true;
	return false;
}

//adjust position in the rope
void TileMap::adjustPosition(glm::ivec2& posPlayer, const glm::ivec2& size, const glm::ivec2& sideMargins, const int& tileAdjust) {
	//int x;
	int ropeTile = tileAdjust;//(posPlayer.x + size.x)/tileSize -1;
	ropeTile *= tileSize;
	ropeTile += (tileSize / 2);
	posPlayer.x = ropeTile - size.x/2;
	/*x *= tileSize;
	int temp = size.x / 2;
	int result = x + (tileSize/2);
	return result;*/
}

bool TileMap::ropeDownstairs(const glm::ivec2 posPlayer, const glm::ivec2& size, int* tileAdjust) const
{
	int x, x1, y;
	x = (posPlayer.x + size.x/2)/ tileSize;
	x1 = (posPlayer.x + size.x - 1) / tileSize;
	y = (posPlayer.y + size.y - 1) / tileSize;
	int tileIndex = map[(y + 1) * mapSize.x + x];
	*tileAdjust = x;
	if (tileIndex == 52) return true;
	return false;
}

bool TileMap::touchTheSurface(const glm::ivec2 posPlayer, const glm::ivec2& size) const {
	int x, x1, y;
	x = (posPlayer.x + size.x / 2) / tileSize;
	x1 = (posPlayer.x + size.x - 1) / tileSize;
	y = (posPlayer.y + size.y - 1) / tileSize;
	int tileIndex = map[y * mapSize.x + x];
	if (tileIndex == 34 ) return true;
	return false;

}


bool TileMap::lastRope(const glm::ivec2 posPlayer, const glm::ivec2& size) const {
	int x, x1, y;
	x = (posPlayer.x + size.x / 2) / tileSize;
	x1 = (posPlayer.x + size.x - 1) / tileSize;
	y = (posPlayer.y + size.y - 1) / tileSize;
	int titleIndex = map[y * mapSize.x + x];
	if ( titleIndex == 52)   return true;
	return false;
}

int TileMap::readNextTile(ifstream& fin) const
{
	char temp;
	fin.get(temp);
	int value = 0;
	while (temp != '-' && temp != '\n') {
		if ( temp != ' ' ) value = value * 10 + (temp - int('0'));// leer integer
        fin.get(temp);
	}
	return value;
}

bool TileMap::enableToMoveRight(const glm::ivec2 posPlayer, const glm::ivec2& size, glm::ivec2 cameraPosition) const {

	int x, y0, y1;

	if (posPlayer.x + size.x > cameraPosition.x + 640 || posPlayer.x < cameraPosition.x) return false;
	x = (posPlayer.x + size.x - 1) / tileSize;
	y0 = posPlayer.y / tileSize;
	y1 = (posPlayer.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		int tileIndex = map[y * mapSize.x + x];
		if (tileIndex == 163)
			return false;
	}
	return diagEMpty(posPlayer, size, false);
}

bool TileMap::enableToMoveLeft(const glm::ivec2 posPlayer, const glm::ivec2& size, glm::ivec2 cameraPosition) const {
	int x, y0, y1;
	if (posPlayer.x > cameraPosition.x + 640 || posPlayer.x < cameraPosition.x) return false;
	x = posPlayer.x / tileSize;
	y0 = posPlayer.y / tileSize;
	y1 = (posPlayer.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		int tileIndex = map[y * mapSize.x + x];
		if (tileIndex == 197)
			return false;
	}
	return diagEMpty(posPlayer, size, true);
}

bool TileMap::diagEMpty(const glm::ivec2 posPlayer, const glm::ivec2& size, bool left) const {
	int diag = (posPlayer.y + (size.y) - 1) / tileSize;
	int x = posPlayer.x / tileSize;
	int aux;
	if (left) aux = map[diag * mapSize.x + x - 1];
	else aux = aux = map[diag * mapSize.x + x + 1];
	if (aux == 1)
		return false;
	return true;
}

bool TileMap::elementHitsTheGround(const glm::ivec2 posRock, const glm::ivec2 size) const {
	int y = posRock.y / tileSize;
	int x = posRock.x / tileSize;
	int tileIndex = map[y * mapSize.x + x];
	if (tileIndex == 33 || tileIndex == 32 || tileIndex == 36) return true;
	return false;
}

int TileMap::getNumTile(int x, int y)const {
	return map[y * mapSize.x + x];
}

bool TileMap::teleportTile(const glm::ivec2 centroid, const glm::ivec2& size) const {
	int x = centroid.x / tileSize;
	int y = centroid.y / tileSize;
	int tileIndex = map[y * mapSize.x + x];
	if (tileIndex == 2) return true;
	return false;
}

bool TileMap::isTileIgnored(const int& tileIndex) {
	return !(tilesToIgnore.find(tileIndex) == tilesToIgnore.end());
}


void TileMap::setTile(const int x, const int y, const int tile) {
	map[y * mapSize.x + x] = tile;
}

void TileMap::updateTiles() {
	prepareArrays(initCoords, shaderProgram);
}

void TileMap::renderExtras() const 
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vaoExtra);
	glEnableVertexAttribArray(posLocationExtra);
	glEnableVertexAttribArray(texCoordLocationExtra);
	glDrawArrays(GL_TRIANGLES, 0, 6 * verticesExtra.size());
	glDisable(GL_TEXTURE_2D);

}

int TileMap::getPrize(const int& x, const int& y) {
	for (pair<glm::ivec2, int> p : doorPrizes) {
		if (p.first.x == x && p.first.y == y) {
			return p.second; 
		}
	}
	return -1;
}

void TileMap::getEnemiesInScene(vector<pair<glm::ivec2, int>>& enemiesTIle, glm::ivec2 cameraPosition) {
	int xLim = cameraPosition.x / tileSize;
	int yLim = cameraPosition.y / tileSize;
	for (int i = yLim; i < yLim + 20; ++i) {
		for (int j = xLim; j < xLim + 32; ++j) {
			int value = map[i * mapSize.x + j];
			if (value == 129 || value == 130 || value == 131 || value == 132 || value == 133)  {
				enemiesTIle.push_back(make_pair(glm::ivec2(j,i), value));
			}
		}
	}
}

void TileMap::getPortalsInScene(vector<glm::ivec2>& posPortals) {
	for (int i = 0; i < mapSize.y ; ++i) {
		for (int j = 0; j < mapSize.x; ++j) {
			int value = map[i * mapSize.x + j];
			if (value == 2) {
				posPortals.push_back(glm::ivec2(j, i));
			}
		}
	}
}

