#ifndef __STUDENT__
#define __STUDENT__

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

/**
* @brief This is the struct that will represent all attributes of the student
*/
typedef struct {
	char *name;
	char *favoriteThing;
	Sprite *sprite;
	Vector4D spriteColor;
}Student;

/**
 * @brief Initializes a student struct with the given values
 * @param name The name of the student
 * @param favoriteThing A char * of the student's favorite thing (just for funsies)
 * @param sprite The sprite of the student
 * @return A pointer to the student
 */
Student * student(char *name, char *favoriteThing, Sprite *sprite);

#endif // !__STUDENT__
