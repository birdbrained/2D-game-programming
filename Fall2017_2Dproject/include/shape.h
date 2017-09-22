#ifndef __SHAPE__
#define __SHAPE__

typedef struct rect
{
	float x, y, w, h;
}Rect;

/**
 * @brief Creates a rectangle structure
 * @param x X position of rect (leftmost)
 * @param y Y position of rect (topmost)
 * @param width Width of rectangle
 * @param height Height of rectangle
 * @returns A Rect pointer to new Rect, NULL if no memory can be allocated
 */
Rect * rect_new(float x, float y, float width, float height);

/**
 * @brief Checks to see if a point is in a Rect
 * @param x X point
 * @param y Y point
 * @param rect The Rect to compare against
 * @returns 1 if point is in Rect, 0 if it's not
 */
int point_in_rect(float x, float y, Rect * rect);

#endif // !__SHAPE__
