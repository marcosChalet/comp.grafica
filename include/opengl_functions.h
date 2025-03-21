#ifndef OPENGL_FUNCTIONS_H
#define OPENGL_FUNCTIONS_H

#include "objects.h"

void draw_point(Point *);
void draw_line(Line *);
void draw_polygon(Polygon *);
void glut_post_redisplay();

#endif