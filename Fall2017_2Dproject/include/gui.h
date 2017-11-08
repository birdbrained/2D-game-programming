#ifndef __GUI__
#define __GUI__

#include <stdlib.h>
#include <SDL.h>
#include "simple_logger.h"

typedef struct guiwindow_s
{
	Uint8 inUse;
	Uint64 id;
	SDL_Rect window;
	SDL_Color windowColor;
}GUIWindow;

void gui_system_init(Uint32 maxGUIs);
GUIWindow * gui_new();
void gui_delete(GUIWindow * window);
void gui_free(GUIWindow * window);
void gui_delete_all();
void gui_draw(GUIWindow * window);
void gui_draw_all();

#endif // !__GUI__
