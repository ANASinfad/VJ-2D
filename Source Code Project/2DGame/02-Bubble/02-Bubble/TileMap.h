#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <set>
#include <list>
#include <vector>


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap * createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program, list<glm::ivec2>& friendsInPosition);
	static TileMap * createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, list<glm::ivec2>& friendsInPosition);
	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }
	bool nearRope(const glm::ivec2 posPlayer, const glm::ivec2& size) const;
	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, int* posX, const glm::ivec2& sideMargins) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, int* posX, const glm::ivec2& sideMargins) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, const glm::ivec2& sideMargins) const;
	bool ropeUpstairs(const glm::ivec2 posPlayer, const glm::ivec2& size,int* tileAdjust) const;
	bool lastRope(const glm::ivec2 posPlayer, const glm::ivec2& size) const;
	bool ropeDownstairs(const glm::ivec2 posPlayer, const glm::ivec2& size,int* tileAdjust) const;
	bool touchTheSurface(const glm::ivec2 posPlayer, const glm::ivec2& size) const;
	void adjustPosition(glm::ivec2& posPlayer, const glm::ivec2& size, const glm::ivec2& sideMargins, const int& tileAdjust);
	int readNextTile(ifstream& fin) const;
	bool diagEMpty(const glm::ivec2 posPlayer, const glm::ivec2& size, bool left) const;
	bool elementHitsTheGround(const glm::ivec2 posRock, const glm::ivec2 size) const;
	int getNumTile(int x, int y) const;
	bool teleportTile(const glm::ivec2 posPlayer, const glm::ivec2& size) const;
	bool enableToMoveLeft(const glm::ivec2 posPlayer, const glm::ivec2& size, glm::ivec2 cameraPosition) const;
	bool enableToMoveRight(const glm::ivec2 posPlayer, const glm::ivec2& size, glm::ivec2 cameraPosition) const;
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY, const glm::ivec2& sideMargins) const;
	bool isTileIgnored(const int& tileIndex);
	void setTile(const int x, const int y, const int value);
	void updateTiles();
	void renderExtras() const;
	int getPrize(const int& x, const int& y);
	void getEnemiesInScene(vector<pair<glm::ivec2, int>>& enemiesTIle, glm::ivec2  cameraPosition);
	void getPortalsInScene(vector<glm::ivec2>& posPortals);
	void setTexProgram(ShaderProgram& program);
	
private:
	bool loadLevel(const string& levelFile, list<glm::ivec2>& friendsInPosition);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao,vaoExtra;
	GLuint vbo, vboExtra;
	GLint posLocation, texCoordLocation, posLocationExtra, texCoordLocationExtra;
	glm::ivec2 position, mapSize, tilesheetSize, initCoords;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
	set<int> tilesToIgnore;
	vector<float> verticesExtra;
	ShaderProgram shaderProgram;
	list<pair<glm::ivec2, int>> doorPrizes;

};


#endif // _TILE_MAP_INCLUDE


