#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <time.h>
#include <fmod.h>
#include <physfs.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "ds_linked_list.h"
#include "ds_priority_queue.h"
#include "ds_graph.h"
#include "tilemap.h"
#include "student.h"
#include "entity_s.h"
#include "formation.h"
#include "audio.h"
//#include "audio_with_fmod.h"
#include "text_s.h"
#include "gui.h"
#include "events.h"
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

#define MAX_TEXT_LENGTH 512

static Sprite *backgroundSprite;
static Sprite *mouse;
static Sprite *mouseSprite;
static Sprite *musicSheet;
static Sprite *controllerIcon;
static Sprite *gui;
static Sprite *eventSprite;
//TileMap *tile_map;
static Entity *pickedUp = NULL;
static Entity *collision = NULL;
static GUIWindow *collisionGUI = NULL;
static Entity *cd;
static Entity *playButton;
static Sprite *guiMarchingStat;
//Graph *fieldGraph;
static int score;
static char consoleText[MAX_TEXT_LENGTH] = "";
static char consoleMarkedText[MAX_TEXT_LENGTH] = "";
static SDL_Rect consoleRect, markedRect;
static TTF_Font *PencilFont;
static SDL_Color colorBlack = { 0, 0, 0, 255 };
static SDL_Color colorWhite = { 255, 255, 255, 255 };
static SDL_Color colorRed = { 255, 0, 0, 255 };
static int cursor = 0;

//game state
int done = 0;
static int currentSet = 0;
static int maxSets = 0;
static Event currEvent = 0;

//other random stuff
int reload = 0;

void cheat_code(char * text)
{
	char buffer[512];
	int n = 0;

	if (!text)
	{
		return;
	}

	if (strncmp(text, "DIE", MAX_TEXT_LENGTH) == 0)
	{
		entityDamageAll(200);
	}
	else if (strncmp(text, "RELOAD", MAX_TEXT_LENGTH) == 0)
	{
		reload = 1;
	}
	else if (strncmp(text, "QUIT", MAX_TEXT_LENGTH) == 0)
	{
		done = 1;
	}
	else if (strncmp(text, "TIME TO SIN", MAX_TEXT_LENGTH) == 0)
	{

	}
}

char * format_set_text(char text[32], int currentSet, int maxSets)
{
	char buffer[5];

	if (!text)
	{
		slog("Error: cannot format a text that is null");
		return NULL;
	}

	strncpy(text, "Set: ", 32);
	snprintf(buffer, 5, "%d", currentSet);
	strcat(text, buffer);
	strcat(text, "/");
	snprintf(buffer, 5, "%d", maxSets);
	strcat(text, buffer);

	return text;
}

void close_level(TileMap * tile_map, Graph * fieldGraph)
{
	if (pickedUp)
	{
		pickedUp = NULL;
	}
	if (gui)
	{
		gui = NULL;
	}
	if (guiMarchingStat)
	{
		guiMarchingStat = NULL;
	}
	if (playButton)
	{
		playButton = NULL;
	}
	if (cd)
	{
		cd = NULL;
	}
	graph_clear(&fieldGraph);
	entityDeleteAll();
	tilemap_clear(tile_map);
	gf2d_sprite_clear_all();
	gui_free_all();
	//soundDeleteAll();
}

Graph * load_level(char * levelFilename, TileMap * tile_map, Graph * fieldGraph, Uint8 closePrevLevel)
{
	char buffer[512];
	char * physBuffer = "";
	int n = 0;
	PHYSFS_File * physFile = NULL;
	FILE * file = NULL;
	FILE * file_temp = NULL;
	if (!levelFilename)
	{
		slog("Error: level file name was null");
		return;
	}
	//fscanf(physFile, "%s", buffer);

	/*file = fopen(levelFilename, "r");
	if (!file)
	{
		slog("Error: cannot load the level with filename (%s)", levelFilename);
		fclose(file);
		return;
	}
	rewind(file);*/

	if (!PHYSFS_exists(levelFilename))
	{
		slog("Error: cannot load the level with filename (%s)");
		return;
	}

	physFile = PHYSFS_openRead(levelFilename);
	physBuffer = (char *)malloc(PHYSFS_fileLength(physFile));
	memset(physBuffer, 0, PHYSFS_fileLength(physFile));
	PHYSFS_readBytes(physFile, physBuffer, PHYSFS_fileLength(physFile));
	PHYSFS_close(physFile);

	if (closePrevLevel > 0)
	{
		close_level(tile_map, fieldGraph);
	}

	//while (fscanf(file, "%s", buffer) != EOF)
	while (sscanf(physBuffer, " %s\n%n", buffer, &n) == 1)
	{
		if (buffer[0] == '~')
		{
			break;
		}
		physBuffer += n;
		if (strcmp(buffer, "background:") == 0)
		{
			//fscanf(file, "%s", buffer);
			sscanf(physBuffer, " %s\n%n", buffer, &n);
			physBuffer += n;
			backgroundSprite = gf2d_sprite_load_image(buffer);
		}
		if (strcmp(buffer, "tilemap:") == 0)
		{
			//fscanf(file, "%s", buffer);
			sscanf(physBuffer, " %s\n%n", buffer, &n);
			physBuffer += n;
			file_temp = fopen(buffer, "r");
			if (!file_temp)
			{
				slog("Error: could not open tilemap file");
				//fclose(file_temp);
				continue;
			}
			tilemap_load_from_file(file_temp, tile_map);
			fclose(file_temp);
			fieldGraph = graph_init_from_tilemap(tile_map, sizeof(Entity));
			graph_print(fieldGraph);
		}
		if (strcmp(buffer, "band:") == 0)
		{
			//fscanf(file, "%s", buffer);
			sscanf(physBuffer, " %s\n%n", buffer, &n);
			physBuffer += n;
			/*file_temp = fopen(buffer, "r");
			if (!file_temp)
			{
				slog("Error: could not open band file");
				//fclose(file_temp);
				continue;
			}*/
			//entityLoadAllFromFile(file_temp, tile_map/*, &fieldGraph*/);
			entityLoadAllFromFile(buffer, tile_map);
			//fclose(file_temp);
			event_execute(currEvent, fieldGraph);
			tilemap_clear_space(&tile_map);
			graph_zero_all(&fieldGraph);
			entityUpdateGraphPositionAll(&fieldGraph);
			score = formation_detect(&fieldGraph);
			currEvent = event_decide();
			event_assign_tiles(&fieldGraph, currEvent, tile_map->height);
			//graph_print(fieldGraph);
			slog("snares (%i) flutes (%i) trumpets (%i) alto saxes (%i) baritones (%i) others (%i)",
				fieldGraph->numSnareDrums,
				fieldGraph->numFlutes,
				fieldGraph->numTrumpets,
				fieldGraph->numAltoSaxes,
				fieldGraph->numBaritones,
				fieldGraph->numOtherInstruments);
		}
		if (strcmp(buffer, "mouse:") == 0)
		{
			//fscanf(file, "%s", buffer);
			sscanf(physBuffer, " %s\n%n", buffer, &n);
			physBuffer += n;
			mouseSprite = gf2d_sprite_load_all(buffer, 32, 32, 16);
			mouse = mouseSprite;
		}
		if (strcmp(buffer, "sets:") == 0)
		{
			sscanf(physBuffer, " %i\n%n", &maxSets, &n);
			physBuffer += n;
			slog("MAX SETS: (%i)", maxSets);
		}
		if (strcmp(buffer, "extraSprites:") == 0)
		{
			while (1)
			{
				//fscanf(file, "%s", buffer);
				sscanf(physBuffer, " %s\n%n", buffer, &n);
				physBuffer += n;
				if (buffer[0] == 'E' && buffer[1] == 'N' & buffer[2] == 'D')
				{
					break;
				}
				gui = gf2d_sprite_load_image(buffer);
			}
		}
		if (strcmp(buffer, "musicSheet") == 0)
		{
			musicSheet = gf2d_sprite_load_image("images/gui/music_sheet.png");
			guiMarchingStat = gf2d_sprite_load_image("images/gui/boot.png");
		}
	}

	controllerIcon = gf2d_sprite_load_all("images/gui/controller64x.png", 64, 64, 1);
	eventSprite = gf2d_sprite_load_all("images/gui/stahp.png", 32, 32, 1);
	//soundAdjustVolumeAll(0);

	//fclose(file);
	return fieldGraph;
}

void input_init()
{
	consoleRect.x = 100;
	consoleRect.y = 100;
	consoleRect.w = 1000;
	consoleRect.h = 50;

	consoleText[0] = 0;
	markedRect = consoleRect;
	consoleMarkedText[0] = 0;

	SDL_StartTextInput();
}

int main(int argc, char * argv[])
{
    /*variable declarations,
	remember, all v. decls. are at the beginning of each function in C*/
    const Uint8 * keys;
    
    int mx,my;
    float mf = 0;
	float guyFrame = 0;
	Sprite *thing;
	Sprite *thing2;
	Sprite *guyx;
	//Sprite *wups;
	Sprite *galSprite;
	Sprite *mehSprite;
	Sprite *closeButton;
	int controllerConnected = 0;

	FILE *tilemapFile;
	int tileClicked = 0;
	int p = 0;

    Vector4D mouseColor = {100,255,255,200};
	Vector2D flipVert = { 0, 1 };
	Vector2D scaleDown = { 0.5, 0.5 };
	Vector2D scaleUp = { 2, 2 };
	Vector2D scaleHalfUp = { 1.5, 1.5 };
	Vector3D rotate = { 16, 16, 0 };
	float SPEEEEEED = 0.1f;

	SDL_Event e;
	SDL_Surface *icon = SDL_LoadBMP("images/sprites/guy16x.bmp");
	TileMap *tile_map;
	Graph *fieldGraph;
	GraphNode *pickedUpNode = NULL;

	FILE *bandFile;
	FILE *levelFile;

	Sound *NJITtheme = NULL;
	Sound *snareDrum = NULL;
	Sound *flute = NULL;
	Sound *trumpet = NULL;
	Sound *altoSax = NULL;
	Sound *baritone = NULL;
	Uint32 musicPlaying = 0;
	//Sound *clap = NULL;
	Sound *cdEject = NULL;

	FMOD_SYSTEM *system;
	FMOD_SOUND *fsound;
	//FSound * test_fsound;

	//TTF_Font *PencilFont;
	//SDL_Color colorBlack = { 0, 0, 0, 255 };
	//SDL_Color colorWhite = { 255, 255, 255, 255 };
	//SDL_Color colorRed = { 255, 0, 0, 255 };
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

	SDL_Surface *statMarchingSurface;
	SDL_Texture *statMarchingTexture;
	int statMarchingX = 0, statMarchingY = 0;
	SDL_Rect statMarchingRect = { 550, 680, 0, 0 };
	char statMarchingText[32];

	SDL_Surface *scoreSurface;
	SDL_Texture *scoreTexture;
	int scoreX = 0, scoreY = 0;
	SDL_Rect scoreRect = { 400, 680, 0, 0 };
	char scoreText[32];

	SDL_Surface *consoleSurface;
	SDL_Texture *consoleTexture;
	int consoleX = 0, consoleY = 0;
	SDL_Rect consoleRectie = { 50, 100, 0, 0 };

	SDL_Surface *setSurface;
	SDL_Texture *setTexture;
	int setW = 0, setH = 0;
	SDL_Rect setRect = { 620, 680, 0, 0 };
	char setText[32];

	Uint8 playButtonPressed = 0;

	GUIWindow * guii;
	GUIWindow * quit_game;
	GUIWindow * options;
	void * guiExtraData = NULL;
	PHYSFS_File * physFile = NULL;
	char * physBuffer= "";
	char token[512] = "";
	int n;

	srand(time(NULL));
	SDL_SetTextInputRect(&consoleRect);

    /*program initializtion*/
    init_logger("dmdwa.log");
    slog("---==== BEGIN ====---");
	PHYSFS_init(NULL);
	PHYSFS_mount("zip/def.zip", "mnt", 1);
	PHYSFS_mount("zip/music.zip", "mnt", 1);
	if (PHYSFS_exists("mnt/test_tiles.png"))
	{
		slog("file exists");
	}
	else
		slog("file does not exist");
	

	physFile = PHYSFS_openRead("mnt/_myBand.band");
	//fscanf(physFile, "%s", physBuffer);
	physBuffer = (char *)malloc(PHYSFS_fileLength(physFile));
	memset(physBuffer, 0, PHYSFS_fileLength(physFile));
	//physBuffer = "";
	PHYSFS_readBytes(physFile, physBuffer, PHYSFS_fileLength(physFile));
	PHYSFS_close(physFile);
	//iterator = (char *)malloc(sizof(physBuffer));
	//strncpy(iterator, physBuffer, sizeof(physBuffer));
	while (sscanf(physBuffer, " %s\n%n", token, &n))
	{
		//puts(token);
		if (token[0] == '~')
		{
			break;
		}
		slog("token (%s)", token);
		physBuffer += n;
	}

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
	audioSystemInit(50, 16, 2, 0, 0, 0);
	soundSystemInit(25);
	text_system_init(50);
	gui_system_init(15);
	//fsound_system_init(20, 100, FMOD_INIT_NORMAL, 0);
	score = 0;
    SDL_ShowCursor(SDL_DISABLE);
	TTF_Init();
	//fileLoadedDude = entityNew();

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
	//musicSheet = gf2d_sprite_load_image("images/gui/music_sheet.png");
	//wups = gf2d_sprite_load_all("images/sprites/wups.png", 32, 32, 2);
	controllerIcon = gf2d_sprite_load_all("images/gui/controller64x.png", 64, 64, 1);
	closeButton = gf2d_sprite_load_all("images/gui/close.png", 25, 25, 1);
	eventSprite = gf2d_sprite_load_all("images/gui/stahp.png", 32, 32, 1);

	tile_map = tilemap_init();
	fieldGraph = graph_init(18, sizeof(Entity));
	fieldGraph = load_level("mnt/level/mainMenu.txt", tile_map, fieldGraph, 0);

	//Load sounds
	//NJITtheme = soundNew("music/bg/NJIT.ogg");
	//NJITtheme = soundLoad("music/bg/NJIT.ogg", 12.0f, 3);
	//slog("do i have a sound? %i", NJITtheme->sound != NULL);
	//soundPlay(NJITtheme, 1, 0, 0, 0);
	//Mix_VolumeChunk(NJITtheme->sound, MIX_MAX_VOLUME); //Use this to change volume on the fly!
	//clap = soundLoad("music/sfx/clap.ogg", 5.0f, 1);
	//cdEject = soundNew(Instrument_Unassigned);
	cdEject = soundLoad("music/sfx/cd_play.ogg", 18.0f, 0, Instrument_Unassigned);

	//snareDrum = soundNew(Instrument_Snare_Drum);
	snareDrum = soundLoad("music/bg/meeeeh-Snare_Drum.ogg", 12.0f, Instrument_Snare_Drum, Instrument_Snare_Drum);
	//flute = soundNew(Instrument_Flute);
	flute = soundLoad("music/bg/meeeeh-Flute.ogg", 12.0f, Instrument_Flute, Instrument_Flute);
	//trumpet = soundNew(Instrument_Trumpet);
	trumpet = soundLoad("music/bg/meeeeh-Bb_Trumpet.ogg", 12.0f, Instrument_Trumpet, Instrument_Trumpet);
	//altoSax = soundNew(Instrument_Alto_Saxophone);
	altoSax = soundLoad("music/bg/meeeeh-Alto_Saxophone.ogg", 12.0f, Instrument_Alto_Saxophone, Instrument_Alto_Saxophone);
	//baritone = soundNew(Instrument_Baritone);
	baritone = soundLoad("music/bg/meeeeh-Baritone.ogg", 12.0f, Instrument_Baritone, Instrument_Baritone);

	FMOD_System_Create(&system);
	FMOD_System_Init(system, 100, FMOD_INIT_NORMAL, 0);
	FMOD_System_CreateSound(system, "mnt/bg/NJIT.ogg", FMOD_DEFAULT, 0, &fsound);
	//FMOD_System_PlaySound(system, fsound, NULL, 0, 0);
	FMOD_System_CreateSound(system, "music/bg/NJIT.ogg", FMOD_DEFAULT, 0, &fsound);
	//FMOD_System_PlaySound(system, fsound, NULL, 0, 0);
	//test_fsound = fsound_load(system, "music/bg/NJIT.ogg", FMOD_DEFAULT, 0, Instrument_Unassigned);
	//fsound_play(system, test_fsound);

	//soundPlay(snareDrum, -1, 1, snareDrum->defaultChannel, 0);
	//soundPlay(flute, -1, 1, flute->defaultChannel, 0);
	//soundPlay(trumpet, -1, 1, trumpet->defaultChannel, 0);
	//soundPlay(altoSax, -1, 1, altoSax->defaultChannel, 0);
	//soundPlay(baritone, -1, 1, baritone->defaultChannel, 0);

	//text testing stuff
	PencilFont = TTF_OpenFont("fonts/Halogen.ttf", 36);
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

	statMarchingSurface = TTF_RenderText_Solid(PencilFont, "", colorWhite);
	statMarchingTexture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), statMarchingSurface);
	SDL_QueryTexture(statMarchingTexture, NULL, NULL, &statMarchingX, &statMarchingY);
	statMarchingRect.w = statMarchingX;
	statMarchingRect.h = statMarchingY;

	snprintf(scoreText, 32, "%d", score);
	scoreSurface = TTF_RenderText_Solid(PencilFont, scoreText, colorRed);
	scoreTexture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), scoreSurface);
	SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreX, &scoreY);
	scoreRect.w = scoreX;
	scoreRect.h = scoreY;

	consoleSurface = TTF_RenderText_Solid(PencilFont, consoleText, colorBlack);
	consoleTexture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), consoleSurface);
	SDL_QueryTexture(consoleTexture, NULL, NULL, &consoleX, &consoleY);
	consoleRectie.w = consoleX;
	consoleRectie.h = consoleY;

	strncpy(setText, format_set_text(setText, currentSet, maxSets), 32);
	setSurface = TTF_RenderText_Solid(PencilFont, setText, colorBlack);
	setTexture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), setSurface);
	SDL_QueryTexture(setTexture, NULL, NULL, &setW, &setH);
	setRect.w = setW;
	setRect.h = setH;

	cd = entityNew();
	cd->mySprite = gf2d_sprite_load_all("images/gui/cd.png", 128, 128, 1);
	cd->position = vector2d(0, 0);
	cd->scale = vector2d(2, 2);
	cd->boundingBox = rect_new(cd->position.x, cd->position.y, 128, 128);

	playButton = entityNew();
	playButton->mySprite = gf2d_sprite_load_image("images/gui/play.png");
	playButton->position = vector2d(64, 256);
	strncpy(playButton->name, "playButton", MAX_CHARS);
	playButton->boundingBox = rect_new(playButton->position.x, playButton->position.y, playButton->mySprite->frame_w, playButton->mySprite->frame_h);
	
	guii = gui_new();
	//guii->sprite = controllerIcon;
	guii->closeButton = closeButton;
	//guii->window.x = 10;
	//guii->window.y = 10;
	guii->window.w = 201;
	guii->window.h = 77;
	guii->windowColor.x = 200;
	guii->windowColor.y = 100;
	guii->windowColor.z = 220;
	guii->windowColor.w = 100;
	guii->position.x = 200;
	guii->position.y = 300;
	guii->padding = 0;
	//strncpy(guii->text, "Hello this is text", GUI_MAX_TEXT_LENGTH);
	guii->font = PencilFont;
	gui_change_text(guii, "Hello this is text\tThis is more text", 300);
	gui_set_closeability(guii, 1);

	quit_game = gui_new();
	quit_game->position.x = 50;
	quit_game->position.y = 600;
	quit_game->windowColor = COLOR_RED_TRANSPARENT;
	quit_game->font = PencilFont;
	gui_change_text(quit_game, "Quit Game", 200);
	quit_game->guiType = GUIType_Button_Quit;
	quit_game->on_click = gui_press_create;
	gui_set_closeability(quit_game, 0);

	options = gui_new();
	options->position.x = 50;
	options->position.y = 560;
	options->windowColor = COLOR_ORANGE;
	options->windowColor.w = 100;
	options->font = PencilFont;
	gui_change_text(options, "Options", 200);
	options->guiType = GUIType_Button_Options;
	options->on_click = gui_press_create;
	gui_set_closeability(options, 0);

	//Input for the console
	//input_init();

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
		//FMOD_System_Update(system);
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
		//e = 0;
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

		if (tile_map)
		{
			tilemap_draw_from_data(tile_map);
		}

		if (pickedUpNode != NULL && pickedUp != NULL)
		{
			graph_draw_walkable_tiles(pickedUpNode, pickedUp->statMarching / 10, tile_map->tileWidth, tile_map->tileHeight, tile_map->xPos, tile_map->yPos, COLOR_RED_TRANSPARENT, 0);
		}

		entityDrawAll();
		entityUpdateAll();
		entityIncrementCurrentFrameAll();
		gui_update_all();
		//snprintf(scoreText, 32, "%d", score);
		//scoreSurface = TTF_RenderText_Solid(PencilFont, scoreText, colorRed);
		//scoreTexture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), scoreSurface);
		//SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreX, &scoreY);
		//scoreRect.w = scoreX;
		//scoreRect.h = scoreY;

		if (pickedUp != NULL)
		{
			draw_line(vector2d(pickedUp->position.x + pickedUp->mySprite->frame_w, pickedUp->position.y + pickedUp->mySprite->frame_h),
						vector2d(mx, my), pickedUp->pathColor);
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
					if (abs(tileClicked / tile_map->width - pickedUp->currentPosition / tile_map->width) + abs(tileClicked % tile_map->width - pickedUp->currentPosition % tile_map->width) > pickedUp->statMarching / 10)
					{
						pickedUp = NULL;
						mouse = mouseSprite;
						surfaceMessage = TTF_RenderText_Solid(PencilFont, "Out of marching range!", colorRed);
					}
					else if (tile_map->space[tileClicked] == 0)
					{
						//slog("poop");
						tile_map->space[pickedUp->currentPosition] = 0;
						tile_map->space[tileClicked] = 1;
						//pickedUp->currentPosition = tileClicked;
						pickedUp->nextPosition = tileClicked;
						mouse = mouseSprite;
						//pickedUp->position.x = (mx - tile_map->xPos) / tile_map->tileWidth * (tile_map->tileWidth);
						//pickedUp->position.y = (my - tile_map->yPos) / tile_map->tileHeight * (tile_map->tileHeight);
						pickedUp = NULL;
						pickedUpNode = NULL;
						//graph_zero_all(&fieldGraph);
						//entityUpdateGraphPositionAll(&fieldGraph);
						//soundAdjustVolumeAll(0);
						//score = formation_detect(&fieldGraph);
						surfaceMessage = TTF_RenderText_Solid(PencilFont, "None selected", colorBlack);
					}
					message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage);
					SDL_QueryTexture(message, NULL, NULL, &texW, &texH);
					rect.w = texW;
					rect.h = texH;

					instrumentSurface = TTF_RenderText_Solid(PencilFont, "", colorBlack);
					instrumentTexture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), instrumentSurface);
					SDL_QueryTexture(instrumentTexture, NULL, NULL, &instX, &instY);
					instrumentRect.w = instX;
					instrumentRect.h = instY;

					statMarchingSurface = TTF_RenderText_Solid(PencilFont, "", colorWhite);
					statMarchingTexture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), statMarchingSurface);
					SDL_QueryTexture(statMarchingTexture, NULL, NULL, &statMarchingX, &statMarchingY);
					statMarchingRect.w = statMarchingX;
					statMarchingRect.h = statMarchingY;
				}
			}
			else if (e.button.button == SDL_BUTTON_LEFT)
			{
				if (playButton != NULL)
				{
					if (point_in_rect(mx, my, playButton->boundingBox))
					{
						//slog("hit da BUTT");
						playButtonPressed = 1;
						soundPlay(cdEject, 0, MIX_MAX_VOLUME / 4, -1, 0);
					}
				}
			}
			e.button.button = 0;
			break;
		case SDL_MOUSEBUTTONUP:
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				collision = entityCheckCollisionInAll(mx, my);
				if (collision != NULL && collision->myInstrument != Instrument_Unassigned)
				{
					slog("collision with guy (%s)", &collision->name);
					if (pickedUp == NULL && collision->currentState != ES_Dead)
					{
						pickedUp = collision;
						mouse = collision->mySprite;
						pickedUpNode = graph_find_node(&fieldGraph, (pickedUp->currentPosition % tile_map->width), (pickedUp->currentPosition / tile_map->width));

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

						snprintf(statMarchingText, 32, "%d", pickedUp->statMarching);
						statMarchingSurface = TTF_RenderText_Solid(PencilFont, statMarchingText, colorWhite);
						statMarchingTexture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), statMarchingSurface);
						SDL_QueryTexture(statMarchingTexture, NULL, NULL, &statMarchingX, &statMarchingY);
						statMarchingRect.w = statMarchingX;
						statMarchingRect.h = statMarchingY;
					}
				}

				//if (point_in_rect(mx, my, tile_map->boundingBox))
				//tileClicked = tilemap_find_tile(mx, my, tile_map);
				//if (tileClicked >= 0)
				//{
				//	slog("collided with tilemap on tile (%i), occupied (%i)", tileClicked, tile_map->space[tileClicked]);
				//}

				collisionGUI = gui_check_collision_in_all(mx, my);
				if (collisionGUI != NULL)
				{
					if (collisionGUI->on_click != NULL)
					{
						guiExtraData = (collisionGUI->on_click)(collisionGUI->guiType, collisionGUI->extraData, collisionGUI->pressed);
						collisionGUI->pressed = 1;
						if ((int)guiExtraData == -1)
						{
							done = 1;
						}
					}
					if (collisionGUI->closeable)
					{
						gui_delete(collisionGUI);
					}
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
		case SDL_KEYUP:
			if (SDL_IsTextInputActive() == SDL_TRUE)
			{
				//slog("key name (%s)", SDL_GetKeyName(e.key.keysym.sym));
				if (strcmp(SDL_GetKeyName(e.key.keysym.sym), "Space") == 0)
				{
					strncat(consoleText, " ", sizeof(consoleText));
				}
				else if (strcmp(SDL_GetKeyName(e.key.keysym.sym), "=") == 0)
				{
					//do nothing!
				}
				else
				{
					strncat(consoleText, SDL_GetKeyName(e.key.keysym.sym), sizeof(consoleText));
				}
				consoleSurface = TTF_RenderText_Solid(PencilFont, consoleText, colorBlack);
				consoleTexture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), consoleSurface);
				SDL_QueryTexture(consoleTexture, NULL, NULL, &consoleX, &consoleY);
				consoleRectie.w = consoleX;
				consoleRectie.h = consoleY;
			}
			switch (e.key.keysym.sym)
			{
			case SDLK_RETURN:
				//slog("pressed enter");
				event_execute(currEvent, fieldGraph);
				graph_zero_all(&fieldGraph);
				tilemap_clear_space(&tile_map);
				entityUpdatePositionAll(tile_map);
				entityUpdateGraphPositionAll(&fieldGraph);
				soundAdjustVolumeAll(0);
				score = formation_detect(&fieldGraph);
				currEvent = event_decide();
				event_assign_tiles(&fieldGraph, currEvent, tile_map->height);
				snprintf(scoreText, 32, "%d", score);
				scoreSurface = TTF_RenderText_Solid(PencilFont, scoreText, colorRed);
				scoreTexture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), scoreSurface);
				SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreX, &scoreY);
				scoreRect.w = scoreX;
				scoreRect.h = scoreY;

				//update current set
				currentSet++;
				strncpy(setText, format_set_text(setText, currentSet, maxSets), 32);
				setSurface = TTF_RenderText_Solid(PencilFont, setText, colorBlack);
				setTexture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), setSurface);
				SDL_QueryTexture(setTexture, NULL, NULL, &setW, &setH);
				setRect.w = setW;
				setRect.h = setH;

				//e.key.keysym.sym = SDLK_UNKNOWN;

				break;
			case SDLK_BACKQUOTE:
				//slog("backtick pressed");
				if (SDL_IsTextInputActive() == SDL_FALSE)
				{
					//SDL_StartTextInput();
					input_init();
				}
				strncpy(consoleText, "", 512);
				break;
			case SDLK_EQUALS:
				if (SDL_IsTextInputActive() == SDL_TRUE)
				{
					SDL_StopTextInput();
					//slog("entered text: %s", consoleText);
					consoleSurface = TTF_RenderText_Solid(PencilFont, "", colorBlack);
					consoleTexture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), consoleSurface);
					SDL_QueryTexture(consoleTexture, NULL, NULL, &consoleX, &consoleY);
					consoleRectie.w = consoleX;
					consoleRectie.h = consoleY;
					//handle_console(gf2d_graphics_get_renderer());

					cheat_code(consoleText);
				}
				break;
			}
			e.key.keysym.sym = SDLK_UNKNOWN;
			break;
		/*case SDL_TEXTINPUT:
			if (SDL_IsTextInputActive() == SDL_TRUE)
			{
				//strcat(consoleText, e.text.text);
				if (strlen(e.text.text) == 0 || e.text.text[0] == '\n' || markedRect.w < 0)
					break;

				slog("Keyboard input: %s", e.text.text);

				if (strlen(consoleText) + strlen(e.text.text) < sizeof(consoleText))
				{
					strncat(consoleText, e.text.text, sizeof(consoleText));
					consoleSurface = TTF_RenderText_Solid(PencilFont, consoleText, colorBlack);
					consoleTexture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), consoleSurface);
					SDL_QueryTexture(consoleTexture, NULL, NULL, &consoleX, &consoleY);
					consoleRectie.w = consoleX;
					consoleRectie.h = consoleY;
				}

				slog("Text inputted: %s", consoleText);

				consoleMarkedText[0] = 0;
				//handle_console(gf2d_graphics_get_renderer());
				break;
			}
			break;*/
		case SDL_TEXTEDITING:
			if (SDL_IsTextInputActive() == SDL_TRUE)
			{
				strncpy(consoleMarkedText, e.edit.text, 512);
			}
			break;
		}

		if (playButtonPressed && cd != NULL)
		{
			cd->position.x += 5;
		}

		if (cd != NULL)
		{
			if (point_in_rect(1000, 10, cd->boundingBox))
			{
				fieldGraph = load_level("mnt/level/myLevel.txt", tile_map, fieldGraph, 1);
				strncpy(setText, format_set_text(setText, currentSet, maxSets), 32);
				setSurface = TTF_RenderText_Solid(PencilFont, setText, colorBlack);
				setTexture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), setSurface);
				SDL_QueryTexture(setTexture, NULL, NULL, &setW, &setH);
				setRect.w = setW;
				setRect.h = setH;
				if (musicPlaying > 0)
				{
					//Mix_RewindMusic();
					Mix_HaltChannel(-1);
				}
				soundPlay(snareDrum, -1, MIX_MAX_VOLUME / 4, snareDrum->defaultChannel, 0);
				soundPlay(flute, -1, MIX_MAX_VOLUME / 4, flute->defaultChannel, 0);
				soundPlay(trumpet, -1, MIX_MAX_VOLUME / 4, trumpet->defaultChannel, 0);
				soundPlay(altoSax, -1, MIX_MAX_VOLUME / 4, altoSax->defaultChannel, 0);
				soundPlay(baritone, -1, MIX_MAX_VOLUME / 4, baritone->defaultChannel, 0);
				soundAdjustVolumeAll(0);
				//soundAdjustVolume(baritone, 0);
				//soundAdjustVolume(snareDrum, 0);
				//soundAdjustVolume(trumpet, 0);
				//slog("baritone volume (%d)", baritone->sound->volume);
				musicPlaying = 1;
			}
		}
		//slog("ds %i %i %i %i", cd->boundingBox->x, cd->boundingBox->y, cd->boundingBox->w, cd->boundingBox->h);

		//UI elements last
		if (musicSheet)
			gf2d_sprite_draw(musicSheet, vector2d(0, 592), &scaleUp, NULL, NULL, NULL, NULL, 0);
		if (gui)
			gf2d_sprite_draw(gui, vector2d(0, 0), &scaleUp, NULL, NULL, NULL, NULL, 0);
		if (guiMarchingStat)
			gf2d_sprite_draw(guiMarchingStat, vector2d(500, 600), &scaleUp, NULL, NULL, NULL, NULL, 0);
		//text_draw_all();
		//text_draw(nameText);
		if (message && musicSheet)
		{
			SDL_RenderCopy(gf2d_graphics_get_renderer(), message, NULL, &rect);
			SDL_RenderCopy(gf2d_graphics_get_renderer(), instrumentTexture, NULL, &instrumentRect);
			SDL_RenderCopy(gf2d_graphics_get_renderer(), scoreTexture, NULL, &scoreRect);
		}
		if (guiMarchingStat)
		{
			SDL_RenderCopy(gf2d_graphics_get_renderer(), statMarchingTexture, NULL, &statMarchingRect);
		}
		if (setTexture && maxSets > 0)
		{
			SDL_RenderCopy(gf2d_graphics_get_renderer(), setTexture, NULL, &setRect);
		}
		//SDL_RenderPresent(renderer);
		//gf2d_sprite_draw_image(textBox, vector2d(50, 50));
		if (controllerConnected && controllerIcon)
			gf2d_sprite_draw(controllerIcon, vector2d(700, 600), &scaleUp, NULL, NULL, NULL, NULL, 0);

		if (consoleTexture)
			SDL_RenderCopy(gf2d_graphics_get_renderer(), consoleTexture, NULL, &consoleRectie);

		//gf2d_sprite_draw(wups, vector2d(200, 200), &scaleUp, NULL, &rotate, NULL, NULL, 0);
		//rotate.z += SPEEEEEED;
		//SPEEEEEED *= 1.005;

		gui_draw_all();
		if (guii->inUse)
		{
			//slog("aaa");
			gui_draw(guii);
		}
		event_draw_from_graph(fieldGraph, eventSprite, tile_map->tileWidth, tile_map->tileHeight, tile_map->xPos, tile_map->yPos);

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
        
		//if (keys[SDL_SCANCODE_Q])
		if (reload)
		{
			//close_level(tile_map);
			fieldGraph = load_level("mnt/level/myLevel.txt", tile_map, fieldGraph, 1);
			strncpy(setText, format_set_text(setText, currentSet, maxSets), 32);
			setSurface = TTF_RenderText_Solid(PencilFont, setText, colorBlack);
			setTexture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), setSurface);
			SDL_QueryTexture(setTexture, NULL, NULL, &setW, &setH);
			setRect.w = setW;
			setRect.h = setH;
			if (musicPlaying > 0)
			{
				//Mix_RewindMusic();
				Mix_HaltChannel(-1);
			}
			soundPlay(snareDrum, -1, MIX_MAX_VOLUME / 4, snareDrum->defaultChannel, 0);
			soundPlay(flute, -1, MIX_MAX_VOLUME / 4, flute->defaultChannel, 0);
			soundPlay(trumpet, -1, MIX_MAX_VOLUME / 4, trumpet->defaultChannel, 0);
			soundPlay(altoSax, -1, MIX_MAX_VOLUME / 4, altoSax->defaultChannel, 0);
			soundPlay(baritone, -1, MIX_MAX_VOLUME / 4, baritone->defaultChannel, 0);
			soundAdjustVolumeAll(0);
			musicPlaying = 1;
			reload = 0;
		}

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
	FMOD_Sound_Release(fsound);
	FMOD_System_Release(system);
	graph_clear(&fieldGraph);
	TTF_Quit();
	SDL_DestroyTexture(message);
	SDL_FreeSurface(surfaceMessage);
	PHYSFS_deinit();
    return 0;
}
/*eol@eof*/
