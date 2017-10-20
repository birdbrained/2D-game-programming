#ifndef __SHAPE__
#define __SHAPE__

#include "gf2d_vector.h"
#include "gf2d_graphics.h"

#define COLOR_WHITE vector4d(255, 255, 255, 255)
#define COLOR_BLACK vector4d(0, 0, 0, 255)
#define COLOR_RED vector4d(255, 0, 0, 255)
#define COLOR_GREEN vector4d(0, 255, 0, 255)
#define COLOR_BLUE vector4d(0, 0, 255, 255)
#define COLOR_YELLOW vector4d(255, 255, 0, 255)
#define COLOR_ORANGE vector4d(255, 155, 0, 255)
#define COLOR_PINK vector4d(255, 155, 255, 255)

typedef struct rect_s
{
	float x, y, w, h;
}Rect;

typedef struct circle_s
{
	float x, y, r;
}Circle;

/**
 * @brief Creates a rectangle structure
 * @param x X position of rect (leftmost)
 * @param y Y position of rect (topmost)
 * @param width Width of rectangle
 * @param height Height of rectangle
 * @returns A Rect pointer to new Rect; NULL if no memory can be allocated
 */
Rect * rect_new(float x, float y, float width, float height);

/**
 * @brief Checks to see if a point is in a Rect
 * @param x X point
 * @param y Y point
 * @param rect The Rect to compare against
 * @returns 1 if point is in Rect; 0 if it's not
 */
int point_in_rect(float x, float y, Rect * rect);

/**
 * @brief Checks to see if two rects are colliding
 * @param a The first rect
 * @param b The second rect
 * @returns 1 if rects are colliding; 0 if they are not
 */
int rect_in_rect(Rect * a, Rect * b);

Circle * circle_new(float x, float y, float radius);

/**
 * @brief Checks to see if a point is in a circle
 * @param x The x pos of the point
 * @param y The y pos of the point
 * @param c The circle to check
 * @returns 1 if point in circle; 0 if it is not or if c was null
 */
int point_in_circle(float x, float y, Circle * c);

void draw_line(Vector2D pointA, Vector2D pointB, Vector4D color);

int random_int(int min, int max);

#endif // !__SHAPE__
