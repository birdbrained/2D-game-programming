#ifndef __FORMATION__
#define __FORMATION__

#include "ds_graph.h"
#include "simple_logger.h"

#define FORM_CHAR_LEN 512

typedef enum formation_e
{
	Formation_None,
	Formation_Line,
	Formation_Rectangle
}Formation;

Formation formation_detect(Graph * graph);
char * formation_to_string(Formation f);

#endif // !__FORMATION__
