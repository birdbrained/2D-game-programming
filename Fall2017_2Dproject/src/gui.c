#include "gui.h"

typedef struct guimanager_s
{
	Uint64 increment;
	Uint32 maxGUIs;
	GUIWindow * guiList;
}GUIManager;

static GUIManager guiManager = { 0, 0, NULL };

void gui_system_close()
{
	int i = 0;

	if (guiManager.guiList != NULL)
	{
		for (i = 0; i < guiManager.maxGUIs; i++)
		{
			gui_free(&guiManager.guiList[i]);
		}
		free(guiManager.guiList);
	}

	memset(&guiManager, 0, sizeof(GUIManager));
	slog("GUI system closed.");
}

void gui_system_init(Uint32 maxGUIs)
{
	if (maxGUIs <= 0)
	{
		slog("Error: cannot initialize a gui system for zero guis");
		return;
	}
	memset(&guiManager, 0, sizeof(GUIManager));

	guiManager.guiList = (GUIWindow *)malloc(sizeof(GUIWindow) * maxGUIs);
	if (!guiManager.guiList)
	{
		slog("Error: could not allocate memory for the gui list");
		//close the gui system here
		return;
	}
	memset(guiManager.guiList, 0, sizeof(GUIWindow) * maxGUIs);
	guiManager.maxGUIs = maxGUIs;

	slog("GUI system initialized");
	atexit(gui_system_close);
}

GUIWindow * gui_new()
{
	int i = 0;

	for (i = 0; i < guiManager.maxGUIs; i++)
	{
		if (guiManager.guiList[i].inUse == 0)
		{
			slog("Found a spot for a gui at (%i)", i);
			memset(&guiManager.guiList[i], 0, sizeof(GUIWindow));
			guiManager.guiList[i].id = guiManager.increment++;
			guiManager.guiList[i].inUse = 1;
			return &guiManager.guiList[i];
		}
	}

	slog("Error: out of gui addresses!");
	return NULL;
}

void gui_delete(GUIWindow * window)
{
	if (!window)
	{
		slog("Error: cannot delete a gui window that does not exist!");
		return;
	}

	if (window->inUse)
	{
		window->inUse = 0;
	}
	memset(window, 0, sizeof(GUIWindow));
	gui_free(window);
}

void gui_free(GUIWindow * window)
{
	if (!window)
	{
		slog("Error: cannot free a gui window that does not exist!");
		return;
	}

	window->inUse = 0;
}

void gui_delete_all()
{
	int i = 0;

	for (i = 0; i < guiManager.guiList; i++)
	{
		gui_delete(&guiManager.guiList[i]);
	}
}

void gui_draw(GUIWindow * window)
{
	if (!window)
	{
		slog("Error: cannot draw a gui that does not exist!");
		return;
	}
	if (!window->inUse)
	{
		slog("Error: cannot draw a gui that is not in use");
		return;
	}
	if (!(&window->window))
	{
		slog("Error: cannot draw a gui that does not have an sdl rect!");
		return;
	}

	if (window->sprite == NULL)
		draw_filled_rect(window->window, window->windowColor);
	else
		gf2d_sprite_draw_image(window->sprite, window->position);

	if (window->closeButton != NULL)
	{
		gf2d_sprite_draw_image(window->closeButton, window->position);
	}

	//if (window->text[0] != '\0')
	if (window->texture != NULL)
	{
		//window->surface = TTF_RenderText_Solid(window->font, window->text, window->textColor);
		//window->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), window->surface);
		//SDL_QueryTexture(window->texture, NULL, NULL, &window->rectW, &window->rectH);
		//window->window.w = window->rectW;
		//window->window.h = window->rectH;
		SDL_RenderCopy(gf2d_graphics_get_renderer(), window->texture, NULL, &window->window);
	}
}

void gui_draw_all()
{
	int i = 0;

	for (i = 0; i < guiManager.maxGUIs; i++)
	{
		if (guiManager.guiList[i].inUse > 0)
		{
			gui_draw(&guiManager.guiList[i]);
		}
	}
}

void gui_update(GUIWindow * window)
{
	if (!window)
	{
		return;
	}

	window->window.x = window->position.x;
	window->window.y = window->position.y;

	//bounding box always is at top-left corner
	//when bounding box is pressed, it acts as the "x" button
	//and closes the window / delete the gui
	//if (window->sprite == NULL)
	//{
		window->boundingBox = rect_new(window->position.x, window->position.y, 25.0f, 25.0f);
	//}
	//else
	//{

	//}
}

void gui_update_all()
{
	int i = 0;

	for (i = 0; i < guiManager.maxGUIs; i++)
	{
		if (guiManager.guiList[i].inUse > 0)
		{
			gui_update(&guiManager.guiList[i]);
		}
	}
}

void gui_change_text(char * text, GUIWindow * window)
{
	strncpy(window->text, text, GUI_MAX_TEXT_LENGTH);
	window->surface = TTF_RenderText_Solid(window->font, window->text, window->textColor);
	window->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), window->surface);
	SDL_QueryTexture(window->texture, NULL, NULL, &window->rectW, &window->rectH);
	window->window.w = window->rectW;
	window->window.h = window->rectH;
}

GUIWindow * gui_check_collision_in_all(int mx, int my)
{
	int i = 0;

	for (i = 0; i < guiManager.maxGUIs; i++)
	{
		if (guiManager.guiList[i].inUse > 0)
		{
			if (point_in_rect(mx, my, guiManager.guiList[i].boundingBox))
			{
				return &guiManager.guiList[i];
			}
		}
	}

	return NULL;
}
