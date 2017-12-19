#ifndef __FORMATION__
#define __FORMATION__

#include "ds_graph.h"
#include "simple_logger.h"
#include "audio.h"

#define FORM_CHAR_LEN 512

typedef enum formation_e
{
	Formation_None,
	Formation_Line,
	Formation_Rectangle
}Formation;

/**
 * @brief Attempts to detect a formation/pattern from a graph
 * @param graph The graph to check
 * @returns The total score of all formations
 */
int formation_detect(Graph ** graph);

/**
 * @briefs Creates a string representation of a certain formation
 * @param f The formation enum to build a string from
 * @returns The character array of the formation; "No formation" if no formation
 */
char * formation_to_string(Formation f);

/**
 * @brief Gets the current staticaly defined formation
 * @returns The current formation
 */
Formation formation_get_curr();

#endif // !__FORMATION__
