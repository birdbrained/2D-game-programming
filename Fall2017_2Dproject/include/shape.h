#ifndef __SHAPE__
#define __SHAPE__

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
int point_in_circle(float x, float y, Circle * c);

#endif // !__SHAPE__
