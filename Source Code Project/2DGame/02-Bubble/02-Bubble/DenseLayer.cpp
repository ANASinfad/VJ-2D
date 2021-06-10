#include "DenseLayer.h"

void DenseLayer::render() {
	int pos = 0;
	for (int j = 0; j < mapHeight; j++)
		for (int i = 0; i < mapWidth; i++, pos++)
			RenderTile(map[pos].index, i * tileWidth, j * tileHeight,
				tileWidth, tileHeight);
}

void RenderTile(int indexOfTile, int Iwidth, int Jheight, int tileWidth, int tileHeight) {

}