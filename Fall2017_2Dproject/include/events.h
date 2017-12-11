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

Event event_decide();
int event_assign_tiles(Graph ** graph, Event e, int height);
void event_damage_entities(int damage, Graph * graph);
int event_execute(Event e, Graph * graph);
int event_draw_from_graph(Graph * graph, Sprite * sprite, int tileWidth, int tileHeight, int xPos, int yPos);

#endif // !__EVENTS__
