#ifndef __TILEMAP__
#define __TILEMAP__

#include <SDL.h>
#include <stdlib.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"

/**
 * @brief Draws a tile map to the screen
 * @param tilemap The sprite containing the tilemap information
 * @param tiles Int array signifying which tile to load on which spot
 * @param width The number of tiles to draw in a line
 * @param height The number of lines of tiles to draw
 * @param xPos Where to starting drawing the tilemap
 * @param yPos Where to starting drawing the tilemap
 * @return 0 if successful
 */
int DrawTileMap(Sprite *tilemap, const int *tiles, unsigned int width, unsigned int height, int xPos, int yPos);

#endif // !__TILEMAP__
