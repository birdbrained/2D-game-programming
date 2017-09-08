#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "ds_linked_list.h"

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
    Vector4D mouseColor = {255,100,255,200};
	Vector2D flipVert = { 0, 1 };
	IntNode *myLL = IntNode_init(5);
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);

	//derp
	slog("%i", myLL->data);
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	thing = gf2d_sprite_load_all("images/ed210.png", 128, 128, 16);
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
		gf2d_sprite_draw(thing, vector2d(10, 10), NULL, NULL, NULL, &flipVert, NULL, (int)mf);

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
