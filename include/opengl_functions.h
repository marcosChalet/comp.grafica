#ifndef OPENGL_FUNCTIONS_H
#define OPENGL_FUNCTIONS_H

#include "objects.h"

void draw_point(Point_d *);
void draw_line(Line_d *);
void draw_polygon(Polygon_d *);
void glut_post_redisplay();

#endif