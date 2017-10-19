#include "text_s.h"

typedef struct textmanager_s
{
	Uint32 maxTextDisplays;
	TextDisplay * textList;
	Uint64 increment;
}TextManager;

static TextManager textManager = { 0, NULL, 0 };

void text_system_close()
{
	int i = 0;

	if (textManager.textList != NULL)
	{
		for (i = 0; i < textManager.maxTextDisplays; i++)
		{
			text_free(&textManager.textList[i]);
		}
		free(textManager.textList);
	}
	memset(&textManager, 0, sizeof(TextManager));
	slog("Text display system closed.");
}

void text_system_init(Uint32 maxTextDisplays)
{
	if (maxTextDisplays <= 0)
	{
		slog("Error: cannot initialize text system for 0 texts");
		return;
	}
	memset(&textManager, 0, sizeof(TextManager));

	textManager.textList = (TextDisplay *)malloc(sizeof(TextDisplay) * maxTextDisplays);
	if (!textManager.textList)
	{
		slog("Error: could not allocate memory for the text list");
		text_system_close();
		return;
	}

	memset(textManager.textList, 0, sizeof(TextDisplay) * maxTextDisplays);
	textManager.maxTextDisplays = maxTextDisplays;

	slog("Text display system initialized");
	atexit(text_system_close);
}

TextDisplay * text_new(TTF_Font * font, char * text, SDL_Color color)
{
	int i = 0;

	for (i = 0; i < textManager.maxTextDisplays; i++)
	{
		if (textManager.textList[i].inUse == 0)
		{
			slog("Found a spot for text at (%i)", i);
			memset(&textManager.textList[i], 0, sizeof(TextDisplay));
			textManager.textList[i].id = textManager.increment++;
			textManager.textList[i].inUse = 1;
			textManager.textList[i].font = &font;
			strncpy(textManager.textList[i].text, text, MAX_TEXT_LENGTH);
			textManager.textList[i].color = color;
			return &textManager.textList[i];
		}
	}
}

void text_delete(TextDisplay * textThatDies)
{
	if (!textThatDies)
	{
		slog("Error: Cannot delete a text that does not exist!");
		return;
	}

	if (textThatDies->inUse)
	{
		textThatDies->inUse = 0;
	}
	memset(textThatDies, 0, sizeof(TextDisplay));
	text_free(textThatDies);
}

void text_free(TextDisplay * t)
{
	if (!t)
	{
		slog("Error: cannot free text that does not exist");
		return;
	}

	t->inUse = 0;
}

void text_delete_all()
{
	int i = 0;

	for (i = 0; i < textManager.maxTextDisplays; i++)
	{
		entityDelete(&textManager.textList[i]);
	}
}

void text_draw(TextDisplay * t)
{
	SDL_Surface * surface;

	t->rect.w = 500;
	t->rect.h = 300;
	surface = TTF_RenderText_Solid(t->font, "placeholdha", t->color);
	t->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surface);
	SDL_QueryTexture(t, NULL, NULL, 500, 500);
	
	SDL_RenderCopy(gf2d_graphics_get_renderer(), &t->text, NULL, &t->rect);
	//SDL_RenderPresent(gf2d_graphics_get_renderer());
}

void text_draw_all()
{
	int i = 0;

	for (i = 0; i < textManager.maxTextDisplays; i++)
	{
		if (textManager.textList[i].inUse > 0)
		{
			text_draw(&textManager.textList[i]);
		}
	}
}
