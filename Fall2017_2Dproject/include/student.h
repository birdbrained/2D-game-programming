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

#endif // !__STUDENT__
