#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "ds_linked_list.h"
#include "ds_priority_queue.h"
#include "tilemap.h"
#include "student.h"
#include "entity_s.h"
#include "audio.h"
#include "text_s.h"
#include "think_functions.h"


/*typedef struct playersave
{
	char name[32];
	int health;
	int ammo;
	int shields;
	int level;
}playerSave;

void save_player_data(playerSave *ps, char *filename)
{
	FILE *file;
	if (!ps)
	{
		return;
	}
	if (!filename)
	{
		return;
	}
	file = fopen(filename, "wb");
	if (!file)
	{
		slog("Failed to open file (%s) for saving", filename);
		return;
	}
	fwrite(ps, sizeof(playerSave), 1, file); //what to write, how big, how many, where to save
	fclose(file);
}

void load_player_data(playerSave *ps, char *filename)
{
	FILE *file;
	if (!ps)
	{
		return;
	}
	if (!filename)
	{
		return;
	}
	file = fopen(filename, "rb");
	if (!file)
	{
		slog("Failed to open file (%s) for reading", filename);
		return;
	}
	fread(ps, sizeof(playerSave), 1, file);
	fclose(file);
}*/

static Sprite *backgroundSprite;
static Sprite *mouse;
static Sprite *mouseSprite;
static Sprite *musicSheet;
static Sprite *controllerIcon;
static Sprite *gui;
static TileMap *tile_map;
static Entity *pickedUp = NULL;
static Entity *collision = NULL;

void close_level()
{
	if (pickedUp)
	{
		pickedUp = NULL;
	}
	entityDeleteAll();
	tilemap_clear(tile_map);
	gf2d_sprite_clear_all();
}

void load_level(char * levelFilename, Uint8 closePrevLevel)
{
	char buffer[512];
	FILE * file = NULL;
	FILE * file_temp = NULL;
	if (!levelFilename)
	{
		slog("Error: level file name was null");
		return;
	}

	file = fopen(levelFilename, "r");
	if (!file)
	{
		slog("Error: cannot load the level with filename (%s)", levelFilename);
		fclose(file);
		return;
	}
	rewind(file);

	if (closePrevLevel > 0)
	{
		close_level();
	}

	while (fscanf(file, "%s", buffer) != EOF)
	{
		if (strcmp(buffer, "background:") == 0)
		{
			fscanf(file, "%s", buffer);
			backgroundSprite = gf2d_sprite_load_image(buffer);
		}
		if (strcmp(buffer, "tilemap:") == 0)
		{
			fscanf(file, "%s", buffer);
			file_temp = fopen(buffer, "r");
			if (!file_temp)
			{
				slog("Error: could not open tilemap file");
				//fclose(file_temp);
				continue;
			}
			tilemap_load_from_file(file_temp, tile_map);
			fclose(file_temp);
		}
		if (strcmp(buffer, "band:") == 0)
		{
			fscanf(file, "%s", buffer);
			file_temp = fopen(buffer, "r");
			if (!file_temp)
			{
				slog("Error: could not open band file");
				//fclose(file_temp);
				continue;
			}
			entityLoadAllFromFile(file_temp);
			fclose(file_temp);
		}
		if (strcmp(buffer, "mouse:") == 0)
		{
			fscanf(file, "%s", buffer);
			mouseSprite = gf2d_sprite_load_all(buffer, 32, 32, 16);
			mouse = mouseSprite;
		}
		if (strcmp(buffer, "extraSprites:") == 0)
		{
			while (1)
			{
				fscanf(file, "%s", buffer);
				if (strcmp(buffer, "END") == 0)
				{
					break;
				}
				gui = gf2d_sprite_load_image(buffer);
			}
		}
	}

	musicSheet = gf2d_sprite_load_image("images/gui/music_sheet.png");
	controllerIcon = gf2d_sprite_load_all("images/gui/controller64x.png", 64, 64, 1);

	fclose(file);
}

int main(int argc, char * argv[])
{
    /*variable declarations,
	remember, all v. decls. are at the beginning of each function in C*/
    int done = 0;
    const Uint8 * keys;
    
    int mx,my;
    float mf = 0;
	float guyFrame = 0;
	Sprite *thing;
	Sprite *thing2;
	Sprite *guyx;
	Sprite *galSprite;
	Sprite *mehSprite;
	int controllerConnected = 0;
	/*Sprite *myTileMap;
	const int level[] = 
	{ 2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 
	  3, 2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 3, 2, 
	  2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3,
	  3, 2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 3, 2,
	  2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 
	  3, 2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 3, 2,
	  2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 
	  3, 2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 3, 2,
	  2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3,
	  3, 2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 3, 2 };*/
	FILE *tilemapFile;
	int tileClicked = 0;
	int p = 0;

    Vector4D mouseColor = {100,255,255,200};
	Vector2D flipVert = { 0, 1 };
	Vector2D scaleDown = { 0.5, 0.5 };
	Vector2D scaleUp = { 2, 2 };
	Vector2D scaleHalfUp = { 1.5, 1.5 };
	//IntNode *myLL = IntNode_init(5);
	/*Student *person;*/
	/*Entity *guy, *testDude;
	Entity *en = NULL;
	Entity *biggo = NULL;
	FILE *infile;
	Entity *fileLoadedDude = NULL;
	Entity *fileLoadedDude2 = NULL;*/
	SDL_Event e;
	SDL_Surface *icon = SDL_LoadBMP("images/sprites/guy16x.bmp");

	FILE *bandFile;
	FILE *levelFile;

	Sound *NJITtheme = NULL;
	Sound *snareDrum = NULL;
	Sound *flute = NULL;
	Sound *trumpet = NULL;
	Sound *altoSax = NULL;
	Sound *tenorSax = NULL;
	//Sound *clap = NULL;

	/*TTF_Font *PencilFont = TTF_OpenFont("fonts/Pencil.ttf", 24);
	if (!PencilFont)
	{
		slog("Error loading font");
	}
	SDL_Color colorBlack = { 255, 255, 255, 255 };
	SDL_Surface *surfaceMessage = TTF_RenderText_Solid(PencilFont, "placeholdha", colorBlack);
	SDL_Texture *message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer, surfaceMessage);
	Sprite *textBox;*/
	TTF_Font *PencilFont;
	SDL_Color colorBlack = { 0, 0, 0, 255 };
	SDL_Surface *surfaceMessage;
	SDL_Texture *message;
	Sprite *textBox;
	TextDisplay *nameText;
	int texW = 0, texH = 0;
	SDL_Rect rect = { 65, 630, 0, 0 };

	SDL_Surface *instrumentSurface;
	SDL_Texture *instrumentTexture;
	int instX = 0, instY = 0;
	SDL_Rect instrumentRect = { 65, 660, 0, 0 };

    /*program initializtion*/
    init_logger("dmdwa.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "Drum Majors Don't Wear Aussies",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0,
		icon);
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); //This line makes images render crisp instead of blurry
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
	entitySystemInit(1024);
	audioSystemInit(50, 10, 2, 0, 0, 0);
	soundSystemInit(25);
	text_system_init(50);
    SDL_ShowCursor(SDL_DISABLE);
	TTF_Init();
	//fileLoadedDude = entityNew();

	//derp
	//slog("%i", myLL->data);
    
    /*demo setup*/
    //backgroundSprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
	//textBox = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouseSprite = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	mouse = mouseSprite;
	//thing = gf2d_sprite_load_all("images/sprites/test_dude.png", 32, 32, 1);
	//thing2 = gf2d_sprite_load_all("images/sprites/test_dude3.png", 64, 64, 1);
	//guyx = gf2d_sprite_load_all("images/sprites/guy32x.png", 32, 32, 2);
	//galSprite = gf2d_sprite_load_all("images/sprites/gal32x.png", 32, 32, 2);
	//mehSprite = gf2d_sprite_load_all("images/sprites/meh32x.png", 32, 32, 2);
	musicSheet = gf2d_sprite_load_image("images/gui/music_sheet.png");
	controllerIcon = gf2d_sprite_load_all("images/gui/controller64x.png", 64, 64, 1);
	//myTileMap = gf2d_sprite_load_all("images/field_tiles.png", 64, 64, 2);
	//person = student("Test", "Sex", thing2);
	//slog("Initializing student %s", person->name);
	/*guy = entityNew();
	strncpy(guy->name, "McBandgeek", 32);
	guy->mySprite = guyx;
	guy->scale = scaleUp;
	guy->currentFrame = 0;
	guy->minFrame = 0;
	guy->maxFrame = 2;
	guy->position = vector2d(300, 100);
	guy->update = move;
	guy->myInstrument = Instrument_Tenor_Saxophone;
	guy->instrumentSprite = gf2d_sprite_load_all("images/sprites/instrument_tenor_sax.png", 32, 32, 1);
	guy->boundingBox = rect_new(guy->position.x, guy->position.y, 64, 64);
	testDude = NULL;
	//SDL_SetTextureColorMod(thing2->texture, 100, 60, 0);
	infile = fopen("def/dude.dude", "r");
	fileLoadedDude = entityNew();
	fileLoadedDude = entityLoadFromFile(infile, fileLoadedDude);
	fclose(infile);
	//fileLoadedDude->mySprite = mehSprite;
	fileLoadedDude->instrumentSprite = gf2d_sprite_load_all(&fileLoadedDude->instrumentSpriteFilePath, 32, 32, 1);
	fileLoadedDude->position = vector2d(64, 64);
	fileLoadedDude->boundingBox = rect_new(fileLoadedDude->position.x, fileLoadedDude->position.y, 64, 64);
	fileLoadedDude->scale = vector2d(2, 2);
	fileLoadedDude->currentFrame = 0;
	fileLoadedDude->minFrame = 0;
	fileLoadedDude->maxFrame = 2;
	fileLoadedDude->currentPosition = 19;
	slog("the thing made has name: %s", &fileLoadedDude->name);

	infile = fopen("def/dude2.dude", "r");
	fileLoadedDude2 = entityNew();
	fileLoadedDude2 = entityLoadFromFile(infile, fileLoadedDude2);
	fclose(infile);
	fileLoadedDude2->instrumentSprite = gf2d_sprite_load_all(&fileLoadedDude2->instrumentSpriteFilePath, 32, 32, 1);
	fileLoadedDude2->position = vector2d(128, 64);
	fileLoadedDude2->boundingBox = rect_new(fileLoadedDude2->position.x, fileLoadedDude2->position.y, 64, 64);
	fileLoadedDude2->scale = vector2d(2, 2);
	fileLoadedDude2->currentFrame = 0;
	fileLoadedDude2->minFrame = 0;
	fileLoadedDude2->maxFrame = 2;
	fileLoadedDude2->currentPosition = 20;*/

	tile_map = tilemap_init();
	load_level("def/level/mainMenu.txt", 0);

	//textBox->texture = message;

	//Trying to load a tilemap from file
	//tilemapFile = fopen("def/level/field_0.tilemap", "r");
	//tilemap_load_from_file(tilemapFile, tile_map);
	//fclose(tilemapFile);
	//slog("tilewidth: (%i) tileheight: (%i) tperline: (%i) filepath: (...) width: (%i) height: (%i) xPos: (%i) yPos: (%i)", tile_map->tileWidth,	tile_map->tileHeight, tile_map->tilesPerLine, tile_map->width, tile_map->height, tile_map->xPos, tile_map->yPos);
	//slog("do i have a sprite? %i", tile_map->tilemapSprite != NULL);
	//tile_map->space[19] = 1;
	//tile_map->space[20] = 1;
	/*slog("tile pq start");
	while (tile_map->tiles_head != NULL)
	{
		p = pq_delete(tile_map->tiles_head, tile_map->tiles_tail);
		if (p == NULL)
		{
			break;
		}
		slog("Removing (%d) from pq", p);
	}
	slog("tile pq end");*/
	/*slog("start array");
	for (p = 0; p < tile_map->width * tile_map->height; p++)
	{
		if (p == 512)
		{
			slog("end of array");
		}
		else if (tile_map->tiles[p] == -1)
		{
			slog("found a -1");
		}
		else
		{
			slog("tiles at index (%i) is (%i)", p, tile_map->tiles[p]);
		}
	}
	slog("end array");*/

	//Trying to load all entities from a file
	//bandFile = fopen("def/_myBand.band", "r");
	//entityLoadAllFromFile(bandFile);
	//fclose(bandFile);

	//Load sounds
	//NJITtheme = soundNew("music/bg/NJIT.ogg");
	//NJITtheme = soundLoad("music/bg/NJIT.ogg", 12.0f, 3);
	//slog("do i have a sound? %i", NJITtheme->sound != NULL);
	//soundPlay(NJITtheme, 1, 0, 0, 0);
	//Mix_VolumeChunk(NJITtheme->sound, MIX_MAX_VOLUME); //Use this to change volume on the fly!
	//clap = soundLoad("music/sfx/clap.ogg", 5.0f, 1);

	snareDrum = soundNew("music/bg/meeeeh-Snare_Drum.ogg");
	snareDrum = soundLoad("music/bg/meeeeh-Snare_Drum.ogg", 12.0f, Instrument_Snare_Drum);
	flute = soundNew("music/bg/meeeeh-Flute.ogg");
	flute = soundLoad("music/bg/meeeeh-Flute.ogg", 12.0f, Instrument_Flute);
	trumpet = soundNew("music/bg/meeeeh-Bb_Trumpet.ogg");
	trumpet = soundLoad("music/bg/meeeeh-Bb_Trumpet.ogg", 12.0f, Instrument_Trumpet);
	altoSax = soundNew("music/bg/meeeeh-Alto_Saxophone.ogg");
	altoSax = soundLoad("music/bg/meeeeh-Alto_Saxophone.ogg", 12.0f, Instrument_Alto_Saxophone);
	tenorSax = soundNew("music/bg/meeeeh-Tenor_Saxophone.ogg");
	tenorSax = soundLoad("music/bg/meeeeh-Tenor_Saxophone.ogg", 12.0f, Instrument_Tenor_Saxophone);

	soundPlay(snareDrum, -1, 1, snareDrum->defaultChannel, 0);
	soundPlay(flute, -1, 1, flute->defaultChannel, 0);
	soundPlay(trumpet, -1, 1, trumpet->defaultChannel, 0);
	soundPlay(altoSax, -1, 1, altoSax->defaultChannel, 0);
	soundPlay(tenorSax, -1, 1, tenorSax->defaultChannel, 0);

	//text testing stuff
	PencilFont = TTF_OpenFont("fonts/Pencil.ttf", 36);
	if (!PencilFont)
	{
		slog("Error loading font");
	}
	surfaceMessage = TTF_RenderText_Solid(PencilFont, "None selected", colorBlack);
	message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage);
	SDL_QueryTexture(message, NULL, NULL, &texW, &texH);
	rect.w = texW;
	rect.h = texH;
	nameText = text_new(PencilFont, "placeholda", colorBlack);
	//slog("nameText inuse (%i)", nameText->inUse);

	instrumentSurface = TTF_RenderText_Solid(PencilFont, "", colorBlack);
	instrumentTexture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), instrumentSurface);
	SDL_QueryTexture(instrumentTexture, NULL, NULL, &instX, &instY);
	instrumentRect.w = instX;
	instrumentRect.h = instY;

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
		SDL_PollEvent(&e);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;        
		guyFrame += 0.05;
		if (guyFrame >= 2.0)guyFrame = 0;
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
		//backgrounds drawn first
		if (backgroundSprite)
		{
			gf2d_sprite_draw_image(backgroundSprite, vector2d(0, 0));
		}

		//Me! trying to add a sprite
		/*tilemap_draw(
			myTileMap,
			level,
			18,
			10,
			0,
			0);*/
		if (tile_map)
		{
			tilemap_draw_from_data(tile_map);
		}

		//gf2d_sprite_draw(thing, vector2d(100, 10), &scaleUp, NULL, NULL, NULL, NULL, 0);
		//gf2d_sprite_draw(thing, vector2d(100, 10), NULL, NULL, NULL, NULL, NULL, 0);
		//gf2d_sprite_draw(guy->mySprite, guy->position, &(guy->scale), NULL, NULL, NULL, NULL, 0);

		/*if (keys[SDL_SCANCODE_W])
		{
			(*guy->update)(guy, vector2d(0, -2));
		}
		if (keys[SDL_SCANCODE_A])
		{
			(*guy->update)(guy, vector2d(-2, 0));
		}
		if (keys[SDL_SCANCODE_S])
		{
			(*guy->update)(guy, vector2d(0, 2));
		}
		if (keys[SDL_SCANCODE_D])
		{
			(*guy->update)(guy, vector2d(2, 0));
		}*/
		//guy->currentFrame = guyFrame;
		//fileLoadedDude->currentFrame = guyFrame;

		/*
		//create an entity if it doesn't exist
		if (keys[SDL_SCANCODE_O] && testDude == NULL)
		{
			testDude = entityNew();
			testDude->mySprite = thing;
			testDude->position = vector2d(500, 500);
			testDude->update = move;
		}
		//if it exists, call its update function
		//slog("%i", testDude != NULL);
		if (testDude != NULL)
		{
			//(*testDude->update)(testDude, vector2d(1, 1));
			gf2d_sprite_draw(testDude->mySprite, testDude->position, NULL, NULL, NULL, NULL, NULL, 0);
		}
		//delete it from memory
		if (keys[SDL_SCANCODE_P] && testDude != NULL)
		{
			entityDelete(testDude);
		}*/
		/*if (keys[SDL_SCANCODE_L] && biggo == NULL)
		{
			biggo = entityNew();
			biggo->mySprite = guyx;
			biggo->position = vector2d(10, 10);
			biggo->scale = vector2d(25, 25);
			biggo->inUse = 1;
			biggo->currentFrame = 0;
			biggo->minFrame = 0;
			biggo->maxFrame = 2;
			biggo->update = move;
			biggo->velocity = vector2d(0.5f, 0.5f);
			biggo->acceleration = vector2d(0.5f, 0.5f);
			biggo->myInstrument = Instrument_Flute;
			biggo->instrumentSprite = gf2d_sprite_load_all("images/sprites/instrument_flute.png", 32, 32, 1);
		}
		if (biggo != NULL)
		{
			//entityDraw(biggo);
			(*biggo->update)(biggo, vector2d(0.5f, 0.5f));
			//biggo->currentFrame = guyFrame;
		}
		if (biggo != NULL && biggo->inUse == 1 && keys[SDL_SCANCODE_P])
		{
			biggo->inUse = 0;
			entityDelete(biggo);
			biggo = NULL;
		}
		if (keys[SDL_SCANCODE_O] && testDude == NULL)
		{
			//slog("Let's make a new thing!");
			testDude = entityNew();
			testDude->mySprite = mehSprite;
			testDude->position = vector2d(200, 200);
			testDude->scale = scaleUp;
			testDude->inUse = 1;
			testDude->currentFrame = 0;
			testDude->minFrame = 1;
			testDude->maxFrame = 3;
			testDude->update = move;
			testDude->instrumentSprite = gf2d_sprite_load_all("images/sprites/instrument_tuba.png", 32, 32, 1);
		}
		if (testDude != NULL)
		{
			//gf2d_sprite_draw(testDude->mySprite, testDude->position, &(testDude->scale), NULL, NULL, NULL, NULL, 0);
			//entityDraw(testDude);
			(*testDude->update)(testDude, vector2d(1, 1));
			//testDude->currentFrame = guyFrame;
		}
		if (testDude != NULL && testDude->inUse == 1 && keys[SDL_SCANCODE_P])
		{
			testDude->inUse = 0;
			entityDelete(testDude);
			testDude = NULL;
		}
		if (keys[SDL_SCANCODE_M] && en == NULL)
		{
			en = entityNew();
			en->mySprite = galSprite;
			en->position = vector2d(300, 500);
			en->scale = vector2d(1,1);
			en->inUse = 1;
			en->currentFrame = 0;
			en->minFrame = 0;
			en->maxFrame = 4;
			en->update = move;
			en->instrumentSprite = gf2d_sprite_load_all("images/sprites/instrument_clarinet.png", 32, 32, 1);
			//soundPlay(clap, 0, clap->volume, clap->defaultChannel, 0);
		}
		if (en != NULL && en->inUse == 1)
		{
			//entityDraw(en);
			(*en->update)(en, vector2d(1, -1));
			//en->currentFrame = guyFrame;
		}
		if (en != NULL && en->inUse == 1 && keys[SDL_SCANCODE_P])
		{
			en->inUse = 0;
			entityDelete(en);
			en = NULL;
		}
		if (en != NULL && en->inUse == 1 && en->position.x >= 400)
		{
			en->inUse = 0;
			entityDelete(en);
			en = NULL;
		}*/

		/*gf2d_sprite_draw(
			guyx,
			vector2d(64, 64),
			&scaleUp,
			NULL,
			NULL,
			NULL,
			NULL,
			0
		);
		gf2d_sprite_draw(
			galSprite,
			vector2d(128, 64),
			&scaleUp,
			NULL,
			NULL,
			NULL,
			NULL,
			0
		);
		gf2d_sprite_draw(
			mehSprite,
			vector2d(192, 64),
			&scaleUp,
			NULL,
			NULL,
			NULL,
			NULL,
			0
		);*/

		//entityDraw(fileLoadedDude);

		entityDrawAll();
		entityUpdateAll();
		entityIncrementCurrentFrameAll();

		if (pickedUp != NULL)
		{
			draw_line(vector2d(pickedUp->position.x + pickedUp->mySprite->frame_w, pickedUp->position.y + pickedUp->mySprite->frame_h),
						vector2d(mx, my), COLOR_RED);
		}

		switch (e.type)
		{
		case SDL_QUIT:
			done = 1;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_RIGHT)
			{
				tileClicked = tilemap_find_tile(mx, my, tile_map);
				if (tileClicked >= 0 && pickedUp != NULL)
				{
					if (tile_map->space[tileClicked] == 0)
					{
						slog("poop");
						tile_map->space[pickedUp->currentPosition] = 0;
						tile_map->space[tileClicked] = 1;
						pickedUp->currentPosition = tileClicked;
						mouse = mouseSprite;
						pickedUp->position.x = (mx - tile_map->xPos) / tile_map->tileWidth * (tile_map->tileWidth);
						pickedUp->position.y = (my - tile_map->yPos) / tile_map->tileHeight * (tile_map->tileHeight);
						pickedUp = NULL;
						surfaceMessage = TTF_RenderText_Solid(PencilFont, "None selected", colorBlack);
						message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage);
						SDL_QueryTexture(message, NULL, NULL, &texW, &texH);
						rect.w = texW;
						rect.h = texH;

						instrumentSurface = TTF_RenderText_Solid(PencilFont, "", colorBlack);
						instrumentTexture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), instrumentSurface);
						SDL_QueryTexture(instrumentTexture, NULL, NULL, &instX, &instY);
						instrumentRect.w = instX;
						instrumentRect.h = instY;
					}
				}
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (e.button.button == SDL_BUTTON_LEFT)
			//if (mousePress(&e.button))
			{
				/*if (point_in_rect(mx, my, guy->boundingBox))
				{
					slog("collision with guy (%s)", guy->name);
				}
				if (point_in_rect(mx, my, fileLoadedDude->boundingBox))
				{
					slog("collision with guy (%s)", &fileLoadedDude->name);
					if (pickedUp == NULL)
					{
						pickedUp = fileLoadedDude;
						mouse = fileLoadedDude->mySprite;
					}
				}
				if (point_in_rect(mx, my, fileLoadedDude2->boundingBox))
				{
					slog("collision with guy (%s)", &fileLoadedDude2->name);
					if (pickedUp == NULL)
					{
						pickedUp = fileLoadedDude2;
						mouse = fileLoadedDude2->mySprite;
					}
				}*/

				collision = entityCheckCollisionInAll(mx, my);
				if (collision != NULL)
				{
					slog("collision with guy (%s)", &collision->name);
					if (pickedUp == NULL)
					{
						pickedUp = collision;
						mouse = collision->mySprite;
						surfaceMessage = TTF_RenderText_Solid(PencilFont, &pickedUp->name, colorBlack);
						message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage);
						SDL_QueryTexture(message, NULL, NULL, &texW, &texH);
						rect.w = texW;
						rect.h = texH;

						instrumentSurface = TTF_RenderText_Solid(PencilFont, entityGetInstrumentName(pickedUp), colorBlack);
						instrumentTexture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), instrumentSurface);
						SDL_QueryTexture(instrumentTexture, NULL, NULL, &instX, &instY);
						instrumentRect.w = instX;
						instrumentRect.h = instY;
					}
				}

				//if (point_in_rect(mx, my, tile_map->boundingBox))
				tileClicked = tilemap_find_tile(mx, my, tile_map);
				if (tileClicked >= 0)
				{
					//slog("collided with tilemap on tile (%i), occupied (%i)", tileClicked, tile_map->space[tileClicked]);
				}
			}
			break;
		case SDL_CONTROLLERDEVICEADDED:
			slog("Connected a controller");
			controllerConnected = 1;
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
			slog("Removed a controller");
			controllerConnected = 0;
			break;
		}

		//UI elements last
		if (musicSheet)
			gf2d_sprite_draw(musicSheet, vector2d(0, 592), &scaleUp, NULL, NULL, NULL, NULL, 0);
		if (gui)
			gf2d_sprite_draw(gui, vector2d(0, 0), &scaleUp, NULL, NULL, NULL, NULL, 0);
		//text_draw_all();
		//text_draw(nameText);
		if (message)
		{
			SDL_RenderCopy(gf2d_graphics_get_renderer(), message, NULL, &rect);
			SDL_RenderCopy(gf2d_graphics_get_renderer(), instrumentTexture, NULL, &instrumentRect);
		}
		//SDL_RenderPresent(renderer);
		//gf2d_sprite_draw_image(textBox, vector2d(50, 50));
		if (controllerConnected && controllerIcon)
			gf2d_sprite_draw(controllerIcon, vector2d(700, 600), &scaleUp, NULL, NULL, NULL, NULL, 0);
		if (pickedUp == NULL)
		{
			gf2d_sprite_draw(
				mouse,				//Sprite to load
				vector2d(mx, my),	//Position to draw it at
				NULL,				//If you want to scale the sprite
				NULL,				//Scale the sprite from a certain position
				NULL,				//Rotation
				NULL,				//Flip
				&mouseColor,		//Color shift
				(int)mf);			//Which frame to draw at
		}
		else
		{
			gf2d_sprite_draw(
				mouse,				//Sprite to load
				vector2d(mx, my),	//Position to draw it at
				&scaleHalfUp,		//If you want to scale the sprite
				NULL,				//Scale the sprite from a certain position
				NULL,				//Rotation
				NULL,				//Flip
				&mouseColor,		//Color shift
				0);			//Which frame to draw at
		}
		gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
		if (keys[SDL_SCANCODE_Q])
		{
			//close_level(tile_map);
			load_level("def/level/myLevel.txt", 1);
		}

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
	TTF_Quit();
	SDL_DestroyTexture(message);
	SDL_FreeSurface(surfaceMessage);
    return 0;
}
/*eol@eof*/
