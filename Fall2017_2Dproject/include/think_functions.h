#include "entity_s.h"
#include "simple_logger.h"
#include "gui.h"

/**
 * @brief Crappy little function that just moves an entity
 * @param move The entity to move
 * @param vec Vector to move entity by
 */
void move(Entity *e, Vector2D vec);

void die_after_time(Entity * e);

/**
 * @brief Checks to see if any mouse button was pressed
 * @param b SDL_MouseButtonEvent to pass in (&event.button)
 * @returns 1 if left mouse button was pressed, 0 if no buttons were pressed
 */
int mousePress(SDL_MouseButtonEvent *b);
//^^^Maybe implement right and middle mouse clicks?

/**
 * @brief When a GUI is pressed, create a GUI based on which GUI was pressed
 * @param self A reference to the GUI that was pressed
 * @returns Can potentially return many different states:
	 0 If nothing happens
	-1 If the quit game button was pressed
	 1 If the character creator will be loaded
	 2 If save button was pressed
	 3 If button to return to main menu was pressed
 */
void * gui_press_create(GUIWindow * self/*GUIType type, void * extraData, Uint8 pressed*/);
