#include <SDL.h>
#include <stdlib.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "tilemap.h"

int DrawTileMap(Sprite * tilemap, const int * tiles, unsigned int width, unsigned int height, int xPos, int yPos)
{
	int tileNum = 0;
	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			//slog("%i", tileNum < sizeof(tiles) / sizeof(const int) ? tiles[tileNum] : 0);
			gf2d_sprite_draw(tilemap, vector2d(xPos + (j * tilemap->frame_w), yPos + (i * tilemap->frame_h)), NULL, NULL, NULL, NULL, NULL, tiles[tileNum]/*tileNum < sizeof(tiles) / sizeof(int) ? tiles[tileNum] : 0*/);
			tileNum += 1;
		}
	}
	return 0;
}
