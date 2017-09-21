#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "ds_linked_list.h"
#include "ds_priority_queue.h"
#include "tilemap.h"
#include "student.h"
#include "entity_s.h"

void move(Entity *e, Vector2D vec)
{
	e->position = vector2d(e->position.x + vec.x, e->position.y + vec.y);
}

int main(int argc, char * argv[])
{
    /*variable declarations,
	remember, all v. decls. are at the beginning of each function in C*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    
    int mx,my;
    float mf = 0;
	float guyFrame = 0;
    Sprite *mouse;
	Sprite *thing;
	Sprite *thing2;
	Sprite *guyx;
	Sprite *galSprite;
	Sprite *mehSprite;
	Sprite *myTileMap;
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
	  3, 2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 3, 2 };
    Vector4D mouseColor = {100,255,255,200};
	Vector2D flipVert = { 0, 1 };
	Vector2D scaleDown = { 0.5, 0.5 };
	Vector2D scaleUp = { 2, 2 };
	IntNode *myLL = IntNode_init(5);
	/*Student *person;*/
	Entity *guy, *testDude;
	Entity *en = NULL;
	Entity *biggo = NULL;
	SDL_Surface *icon = SDL_LoadBMP("images/sprites/guy16x.bmp");
    
    /*program initializtion*/
    init_logger("gf2d.log");
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
    SDL_ShowCursor(SDL_DISABLE);

	//derp
	//slog("%i", myLL->data);
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	thing = gf2d_sprite_load_all("images/sprites/test_dude.png", 32, 32, 1);
	thing2 = gf2d_sprite_load_all("images/sprites/test_dude3.png", 64, 64, 1);
	guyx = gf2d_sprite_load_all("images/sprites/guy32x.png", 32, 32, 2);
	galSprite = gf2d_sprite_load_all("images/sprites/gal32x.png", 32, 32, 2);
	mehSprite = gf2d_sprite_load_all("images/sprites/meh32x.png", 32, 32, 2);
	myTileMap = gf2d_sprite_load_all("images/field_tiles.png", 64, 64, 2);
	//person = student("Test", "Sex", thing2);
	//slog("Initializing student %s", person->name);
	guy = entityNew();
	guy->mySprite = guyx;
	guy->scale = scaleUp;
	guy->currentFrame = 0;
	guy->position = vector2d(100, 100);
	guy->update = move;
	guy->myInstrument = Instrument_Flute;
	guy->instrumentSprite = gf2d_sprite_load_all("images/sprites/instrument_flute.png", 32, 32, 1);
	testDude = NULL;
	//SDL_SetTextureColorMod(thing2->texture, 100, 60, 0);

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;        
		guyFrame += 0.05;
		if (guyFrame >= 2.0)guyFrame = 0;
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
		//backgrounds drawn first
		gf2d_sprite_draw_image(sprite,vector2d(0,0));

		//Me! trying to add a sprite
		DrawTileMap(
			myTileMap,
			level,
			18,
			10,
			0,
			0);
		//gf2d_sprite_draw(thing, vector2d(100, 10), &scaleUp, NULL, NULL, NULL, NULL, 0);
		//gf2d_sprite_draw(thing, vector2d(100, 10), NULL, NULL, NULL, NULL, NULL, 0);
		//gf2d_sprite_draw(guy->mySprite, guy->position, &(guy->scale), NULL, NULL, NULL, NULL, 0);

		if (keys[SDL_SCANCODE_W])
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
		}
		guy->currentFrame = guyFrame;

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
		if (keys[SDL_SCANCODE_L] && biggo == NULL)
		{
			biggo = entityNew();
			biggo->mySprite = guyx;
			biggo->position = vector2d(10, 10);
			biggo->scale = vector2d(25, 25);
			biggo->inUse = 1;
			biggo->currentFrame = 0;
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
			biggo->currentFrame = guyFrame;
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
			testDude->update = move;
			testDude->instrumentSprite = gf2d_sprite_load_all("images/sprites/instrument_clarinet.png", 32, 32, 1);
		}
		if (testDude != NULL)
		{
			//gf2d_sprite_draw(testDude->mySprite, testDude->position, &(testDude->scale), NULL, NULL, NULL, NULL, 0);
			//entityDraw(testDude);
			(*testDude->update)(testDude, vector2d(1, 1));
			testDude->currentFrame = guyFrame;
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
			en->update = move;
			en->instrumentSprite = gf2d_sprite_load_all("images/sprites/instrument_clarinet.png", 32, 32, 1);
		}
		if (en != NULL)
		{
			//entityDraw(en);
			(*en->update)(en, vector2d(1, -1));
			en->currentFrame = guyFrame;
		}
		if (en != NULL && en->inUse == 1 && keys[SDL_SCANCODE_P])
		{
			en->inUse = 0;
			entityDelete(en);
			en = NULL;
		}

		gf2d_sprite_draw(
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
		);

		entityDrawAll();

		//UI elements last
		gf2d_sprite_draw(
			mouse,				//Sprite to load
			vector2d(mx,my),	//Position to draw it at
			NULL,				//If you want to scale the sprite
			NULL,				//Scale the sprite from a certain position
			NULL,				//Rotation
			NULL,				//Flip
			&mouseColor,		//Color shift
			(int)mf);			//Which frame to draw at
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
