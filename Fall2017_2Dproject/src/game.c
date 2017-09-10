#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "ds_linked_list.h"
#include "tilemap.h"
#include "student.h"

int main(int argc, char * argv[])
{
    /*variable declarations,
	remember, all v. decls. are at the beginning of each function in C*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
	Sprite *thing;
	Sprite *thing2;
	Sprite *myTileMap;
	const int level[] = 
	{ 0, 2, 1, 3, 0, 1,
	  1, 3, 1, 1, 2, 2,
	  0, 0, 1, 2, 3, 3 };
    Vector4D mouseColor = {255,100,255,200};
	Vector2D flipVert = { 0, 1 };
	Vector2D scaleDown = { 0.5, 0.5 };
	Vector2D scaleUp = { 4, 4 };
	IntNode *myLL = IntNode_init(5);
	Student *person;
    
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
        0);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); //This line makes images render crisp instead of blurry
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);

	//derp
	//slog("%i", myLL->data);
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	thing = gf2d_sprite_load_all("images/sprites/test_dude.png", 32, 32, 1);
	thing2 = gf2d_sprite_load_all("images/sprites/test_dude3.png", 64, 64, 1);
	myTileMap = gf2d_sprite_load_all("images/test_tiles.png", 32, 32, 4);
	person = student("Test", "Sex", thing2);
	slog("fav thing: %s", person->favoriteThing);
	//person->name = "Tester";
	//person->favoriteThing = "Sexels";
	//person->sprite = thing2;

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;        
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
		//backgrounds drawn first
		gf2d_sprite_draw_image(sprite,vector2d(0,0));

		//Me! trying to add a sprite
		DrawTileMap(
			myTileMap,
			level,
			6,
			3,
			10,
			400);
		gf2d_sprite_draw(thing, vector2d(100, 10), &scaleUp, NULL, NULL, NULL, NULL, 0);
		gf2d_sprite_draw(thing, vector2d(100, 10), NULL, NULL, NULL, NULL, NULL, 0);
		gf2d_sprite_draw(thing2, vector2d(100, 100), NULL, NULL, NULL, NULL, NULL, 0);

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
