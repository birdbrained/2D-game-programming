#include "entity_s.h"

/**
 * @brief Crappy little function that just moves an entity
 * @param move The entity to move
 * @param vec Vector to move entity by
 */
void move(Entity *e, Vector2D vec);

/**
 * @brief Checks to see if any mouse button was pressed
 * @param b SDL_MouseButtonEvent to pass in (&event.button)
 * @returns 1 if left mouse button was pressed, 0 if no buttons were pressed
 */
int mousePress(SDL_MouseButtonEvent *b);
//^^^Maybe implement right and middle mouse clicks?