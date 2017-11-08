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
