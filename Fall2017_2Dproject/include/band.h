#ifndef __BAND__
#define __BAND__

#include <SDL.h>
#include <stdlib.h>
#include "entity_s.h"
#include "simple_logger.h"

typedef struct band_s {
	Uint32 numMembers;
	Entity * myBand;
}Band;

Band * band_init(Uint32 numMembers);
Band * band_load_from_file(FILE * file);

#endif // !__BAND__
