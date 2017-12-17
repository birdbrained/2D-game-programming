#ifndef __EVENTS__
#define __EVENTS__

#include "simple_logger.h"
#include "shape.h"
#include "ds_graph.h"
#include "entity_s.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

#define DAMAGE_FROM_FOOTBALL_PLAYER 20	/**<How much an attack from a football player will deal*/

/**
 * @brief An enum representing the different events that can happen
 */
typedef enum _event_e
{
	Event_None				= 0,
	Event_Football			= 1,
	Event_Colorguard_Flags	= 2,
	Event_Max_Number
}Event;

/**
 * @brief Decides a random event that will happen each round
 * @returns A random Event
 */
Event event_decide();

/**
 * @brief Assigns a given event to occur on a random number of tiles
 * @param graph The graph to effect
 * @param e The event that will occur
 * @param height The height of the graph
 * @returns 0 if successful; -1 if graph was NULL
 */
int event_assign_tiles(Graph ** graph, Event e, int height);

/**
 * @brief Damages all entities that are standing on a certain event tile
 * @param damage The amount of damage to deal
 * @param graph The graph to search for entities on
 */
void event_damage_entities(int damage, Graph * graph);

/**
 * @brief Executes the current event
 * @param e The Event to execute
 * @param graph The graph to search for entities on
 * @returns 0 if successful; -1 if graph was NULL
 */
int event_execute(Event e, Graph * graph);

/**
 * @brief Draws a warning label on all tiles of a graph that will have an event on them
 * @param graph The graph to traverse
 * @param sprite The warning sprite that will be drawn
 * @param tileWidth The width of one tile
 * @param tileHeight The height of one tile
 * @param xPos The starting x position of the tilemap
 * @param yPos The starting y position of the tilemap
 * @returns 0 if successful; -1 if graph was NULL
 */
int event_draw_from_graph(Graph * graph, Sprite * sprite, int tileWidth, int tileHeight, int xPos, int yPos);

#endif // !__EVENTS__
