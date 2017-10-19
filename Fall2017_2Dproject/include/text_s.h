#ifndef __TEXT_S__
#define __TEXT_S__

#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "gf2d_graphics.h"
#include "gf2d_vector.h"
#include "simple_logger.h"

#define MAX_TEXT_LENGTH 512

typedef struct textdisplay_s
{
	short unsigned int inUse;
	Uint64 id;
	TTF_Font * font;
	SDL_Surface * surface;
	char text[MAX_TEXT_LENGTH];
	Vector2D position;
	SDL_Color color;
	SDL_Texture * texture;
	SDL_Rect rect;
}TextDisplay;

void text_system_init(Uint32 maxTextDisplays);
TextDisplay * text_new(TTF_Font * font, char * text, SDL_Color color);
void text_delete(TextDisplay * textThatDies);
void text_free(TextDisplay * t);
void text_delete_all();
void text_draw(TextDisplay * t);
void text_draw_all();

#endif // !__TEXT_S__
