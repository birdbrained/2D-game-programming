#ifndef __GUI__
#define __GUI__

#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "gf2d_draw.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "shape.h"

#define GUI_MAX_TEXT_LENGTH 512

/**
 * @brief A struct that represents a GUI window
 */
typedef struct guiwindow_s
{
	Uint8 inUse;			/**<Don't touch this!*/
	Uint64 id;				/**<ID of the gui. Probably shouldn't touch this.*/
	Sprite * sprite;		/**<Optional. Changes background from a plain rectangle to a sprite.*/
	Sprite * closeButton;	/**<Optional, but really should include. 25x25 sprite for a close button.*/
	Uint8 closeable;		/**<Non-zero if GUI will have a close icon; 0 if not*/
	SDL_Rect window;		/**<The size of the rectangle gui.*/
	Vector4D windowColor;	/**<The background color of a gui window. Applies to sprites and standard windows.*/
	Rect * boundingBox;		/**<Refers to bounding box of the close button.*/
	Vector2D position, scale, scaleCenter, rotation, flip;
	float currentFrame;

	char text[GUI_MAX_TEXT_LENGTH];		/**<Text that will be included in a gui. TODO*/
	TTF_Font * font;
	SDL_Color textColor;
	SDL_Surface * surface;
	SDL_Texture * texture;
	int rectW, rectH;
	int padding;
}GUIWindow;

/**
 * @brief Initializes the GUI system
 * @param maxGUIS The maximum number of GUIs that can exist at once
 */
void gui_system_init(Uint32 maxGUIs);

/**
 * @brief Creates a new GUI
 * @returns A reference to a new GUI on success; NULL if no more space for GUIs
 */
GUIWindow * gui_new();

/**
 * @brief Deletes a GUI
 * @param window The GUI window to delete
 */
void gui_delete(GUIWindow * window);

/**
 * @brief Marks a GUI as free, meaning a new GUI can use the memory;
 * It will not delete a GUI from memory, but the GUI that is freed will no longer be able to update or draw
 */
void gui_free(GUIWindow * window);

/**
 * @brief Deletes all GUIs from memory, does not free memory back to the computer (that is handled by a function
 * in gui.c that automatically gets called on shut-down)
 */
void gui_delete_all();

/**
 * @brief Draws a GUI onto the screen
 * @param window The GUI to draw
 */
void gui_draw(GUIWindow * window);

/**
 * @brief Draws all GUIs currently in use
 */
void gui_draw_all();

/**
 * @brief Updates a GUI
 * @param window The GUI window to update
 */
void gui_update(GUIWindow * window);

/**
 * @brief Updates all GUIs currently in use
 */
void gui_update_all();

/**
 * @brief Changes the text to be displayed on a GUI
 * @param window The GUI window to add text onto
 * @param text The text to insert onto the GUI
 * @param wrapLength The length of the line before moving onto the next one
 */
void gui_change_text(GUIWindow * window, char * text, Uint32 wrapLength);

/**
 * @brief Sets if a GUI window should be closeable from a user clicking the "X" button
 * @param window The GUI window to edit
 * @param closeable Set to 0 if user should not be able to manually close a window;
 *                  Set to non-zero if user should be able to manually close a window
 */
void gui_set_closeability(GUIWindow * window, Uint8 closeable);

/**
 * @brief Checks for a collision with the specified x and y value and all of the GUIs currently in use
 * @param mx The x value to check the collision in
 * @param my The y value to check the collision in
 * @returns A pointer to the GUIWindow in the specified point on success;
 *          NULL if no collision was found
 */
GUIWindow * gui_check_collision_in_all(int mx, int my);

#endif // !__GUI__
