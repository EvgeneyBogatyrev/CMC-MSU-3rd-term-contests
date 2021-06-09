#include <stdio.h>

const double RECT_LEFT = 1.0;
const double RECT_RIGHT = 7.0;
const double RECT_BOTTOM = 2.0;
const double RECT_TOP = 5.0;

const double TRIANGLE_LEFT = 5.0;
const double TRIANGLE_RIGHT = 7.0;
const double TRIANGLE_BOTTOM = 3.0;
const double TRIANGLE_TOP = 5.0;

enum { ARG_NUM = 2 };

int
in_rectangle(
        double x, 
        double y,
        double rect_left,
        double rect_right, 
        double rect_bottom,
        double rect_top)
{
    return x >= rect_left && x <= rect_right 
            && y >= rect_bottom && y <= rect_top;
}

int
in_triangle(
        double x,
        double y,
        double triangle_left,
        double triangle_right,
        double triangle_bottom, 
        double triangle_top)
{
    return in_rectangle(x, y, triangle_left, triangle_right, triangle_bottom, triangle_top)
            && y > triangle_top - (x - triangle_left);
}

int 
main(void)
{
    double x, y;
    if (scanf("%lf%lf", &x, &y) != ARG_NUM) {
        fprintf(stderr, "Wrong number of arguments\n");
        return 1;
    }

    int result = in_rectangle(x, y, RECT_LEFT, RECT_RIGHT, RECT_BOTTOM, RECT_TOP)
            && !in_triangle(x, y, TRIANGLE_LEFT, TRIANGLE_RIGHT, TRIANGLE_BOTTOM, TRIANGLE_TOP);

    printf("%d\n", result);
    return 0;
}
