#include <SDL.h>
#include <stdlib.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "tilemap.h"

TileMap * tilemap_init()
{
	TileMap *new_tilemap = NULL;
	new_tilemap = malloc(sizeof(TileMap));
	if (!new_tilemap)
	{
		slog("Error: could not allocate memory for a TileMap");
		return NULL;
	}
	memset(new_tilemap, 0, sizeof(TileMap));
	memset(new_tilemap->tiles, -1, sizeof(new_tilemap->tiles));
	
	new_tilemap->boundingBox = malloc(sizeof(Rect));
	if (!new_tilemap->boundingBox)
	{
		slog("Error: could not allocate memory for TileMap's boundingBox");
		return NULL;
	}
	memset(new_tilemap->boundingBox, 0, sizeof(Rect));

	return new_tilemap;
}

void tilemap_load_from_file(FILE * file, TileMap * new_tilemap)
{
	char buffer[512];
	//PriorityQueue * tiles_head;
	//PriorityQueue * tiles_tail;
	int tile_i = -1;
	int i = 0;
	if (!file)
	{
		slog("Error: could not open tilemap file");
		return;
	}
	rewind(file);
	//tiles_head = pq_new(sizeof(int));
	//tiles_tail = pq_new(sizeof(int));

	while (fscanf(file, "%s", buffer) != EOF)
	{
		if (strcmp(buffer, "tilewidth:") == 0)
		{
			fscanf(file, "%i", &new_tilemap->tileWidth);
			continue;
		}
		if (strcmp(buffer, "tileheight:") == 0)
		{
			fscanf(file, "%i", &new_tilemap->tileHeight);
			continue;
		}
		if (strcmp(buffer, "tperline:") == 0)
		{
			fscanf(file, "%i", &new_tilemap->tilesPerLine);
			continue;
		}
		if (strcmp(buffer, "filepath:") == 0)
		{
			fscanf(file, "%s", buffer);
			//myTileMap = gf2d_sprite_load_all("images/field_tiles.png", 64, 64, 2);
			new_tilemap->tilemapSprite = gf2d_sprite_load_all(buffer, new_tilemap->tileWidth, new_tilemap->tileHeight, new_tilemap->tilesPerLine);
			continue;
		}
		if (strcmp(buffer, "width:") == 0)
		{
			fscanf(file, "%i", &new_tilemap->width);
			continue;
		}
		if (strcmp(buffer, "height:") == 0)
		{
			fscanf(file, "%i", &new_tilemap->height);
			continue;
		}
		if (strcmp(buffer, "xPos:") == 0)
		{
			fscanf(file, "%i", &new_tilemap->xPos);
			continue;
		}
		if (strcmp(buffer, "yPos:") == 0)
		{
			fscanf(file, "%i", &new_tilemap->yPos);
			continue;
		}
		if (strcmp(buffer, "tiles:") == 0)
		{
			/*while (1)
			{
				fscanf(file, "%i", &tile_i);
				if (tile_i == -1)
				{
					break;
				}
				else
				{
					pq_insert(tiles_head, tiles_tail, tile_i, sizeof(tile_i), tile_i);
				}
			}
			new_tilemap->tiles_head = tiles_head;
			new_tilemap->tiles_tail = tiles_tail;
			continue;*/
			for (i = 0; i < new_tilemap->height * new_tilemap->width; i++)
			{
				fscanf(file, "%i", &tile_i);
				if (i > 511)
				{
					break;
				}
				else if (tile_i == -1)
				{
					new_tilemap->tiles[i] = -1;
					break;
				}
				else
				{
					new_tilemap->tiles[i] = tile_i;
				}
			}
			continue;
		}
	}

	new_tilemap->boundingBox->x = new_tilemap->xPos;
	new_tilemap->boundingBox->y = new_tilemap->yPos;
	new_tilemap->boundingBox->w = new_tilemap->tileWidth * new_tilemap->width;
	new_tilemap->boundingBox->h = new_tilemap->tileHeight * new_tilemap->height;
}

int tilemap_draw(Sprite * tilemap, const int * tiles, unsigned int width, unsigned int height, int xPos, int yPos)
{
	int tileNum = 0;
	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			//slog("%i", tileNum < sizeof(tiles) / sizeof(const int) ? tiles[tileNum] : 0);
			gf2d_sprite_draw(tilemap, vector2d(xPos + (j * tilemap->frame_w), yPos + (i * tilemap->frame_h)), NULL, NULL, NULL, NULL, NULL, tiles[tileNum]/*tileNum < sizeof(tiles) / sizeof(int) ? tiles[tileNum] : 0*/);
			tileNum++;
		}
	}
	return 0;
}

int tilemap_draw_from_data(TileMap * tilemap)
{
	unsigned int i = 0;
	unsigned int j = 0;
	int tileNum;
	if (!tilemap)
	{
		slog("Error: no tilemap");
		return -1;
	}
	/*if (!tilemap->tiles_head || !tilemap->tiles_tail)
	{
		slog("Error: incomplete pq in tilemap");
		return;
	}*/
	tileNum = 0;
	for (i = 0; i < tilemap->height; i++)
	{
		for (j = 0; j < tilemap->width; j++)
		{
			gf2d_sprite_draw(tilemap->tilemapSprite, vector2d(tilemap->xPos + (j * tilemap->tilemapSprite->frame_w), tilemap->yPos + (i * tilemap->tilemapSprite->frame_h)), NULL, NULL, NULL, NULL, NULL, tilemap->tiles[tileNum]);
			tileNum++;
		}
	}
	return 0;
}

int tilemap_find_tile(int mx, int my, TileMap * map)
{
	int x = 0;
	int y = 0;
	int index = 0;

	if (point_in_rect(mx, my, map->boundingBox))
	{
		//slog("xPos (%i) yPos (%i) tileWidth (%i) tileHeight (%i) \nmx (%d) my (%d)", map->xPos, map->yPos, map->tileWidth, map->tileHeight, mx, my);
		mx = mx - map->xPos;
		my = my - map->yPos;
		x = mx / map->tileWidth;
		y = my / map->tileHeight;
		index = x + (y * map->width);
		//slog("in tile x (%i) y (%i) index (%i)", x, y, index);
		return index;
	}

	return -1;
}
