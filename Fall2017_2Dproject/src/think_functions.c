#include "think_functions.h"

void move(Entity *e, Vector2D vec)
{
	if (!e)
	{
		return;
	}
	e->position = vector2d(e->position.x + vec.x, e->position.y + vec.y);
	/*if (e->boundingBox != NULL)
	{
	e->boundingBox->x = e->position.x;
	e->boundingBox->y = e->position.y;
	}*/
}

void die_after_time(Entity * e)
{
	if (!e)
	{
		slog("Cannot run die_after_time function for an entity that does not exist");
		return;
	}
	if (e->inUse == 0)
	{
		slog("Cannot run die_after_time function for an entity that is not in use");
		return;
	}
	if (SDL_GetTicks() % 50 == 0)
	{
		e->inUse = 0;
		entityDelete(e);
		e = NULL;
	}
}

int mousePress(SDL_MouseButtonEvent *b)
{
	if (b->button == SDL_BUTTON_LEFT)
	{
		return 1;
	}
	return 0;
}

void * gui_press_create(GUIWindow * self/*GUIType type, void * extraData, Uint8 pressed*/)
{
	GUIWindow * new_gui = NULL;

	if (!self)
	{
		return;
	}
	if (self->pressed)
	{
		return 0;
	}

	new_gui = gui_new();
	if (!new_gui)
	{
		return NULL;
	}

	switch (self->guiType)
	{
	case GUIType_Button_Quit:
		gui_free(new_gui);
		return -1;
		break;
	case GUIType_Button_Controls:
		new_gui->position = vector2d(300, 50);
		new_gui->windowColor = COLOR_PURPLE;
		new_gui->font = self->font;
		new_gui->padding = 5;
		new_gui->closeButton = (Sprite *)self->extraData;
		new_gui->guiType = GUIType_Button_Controls_Close;
		gui_change_text(new_gui, "Use the mouse to move the cursor. Select a band member to control with the Left Mouse Button. Place a band member with the Right Mouse Button. Move to the next set with Enter.", 600);
		gui_set_closeability(new_gui, 1);
		new_gui->on_click = gui_press_create;
		break;
	case GUIType_Button_Controls_Close:
		gui_free(new_gui);
		//self->on_click(GUIType_Button_Controls, 0);
		gui_find_and_set_pressed_state(GUIType_Button_Controls, 0);
		break;
	case GUIType_Button_CC:
		gui_free(new_gui);
		return 1;
		break;
	}
}


