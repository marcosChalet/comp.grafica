#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdarg.h>
#include "objects_data_structure.h"

typedef enum {
    POINT,
    LINE,
    POLYGON,
} Objec_t;

typedef struct point {
    int x, y;
    void (*draw)(struct point *);
    char * (*to_string)(struct point *);
} Point;

typedef struct line {
    Point * s_point;
    Point * e_point;
    void (*draw)(struct line *);
    char * (*to_string)(struct line *);
} Line;

typedef struct polygon {
    Structure * vertices;
    void (*draw)(struct polygon *);
    char * (*to_string)(struct polygon *);
} Polygon;

void * create_point(Point *);
void * create_line(Point *);
void * create_polygon(Point *);

#endif