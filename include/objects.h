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
    char * (*to_string)(const struct point *);
} Point;

typedef struct line {
    Point * s_point;
    Point * e_point;
    char * (*to_string)(const struct line *);
} Line;

typedef struct polygon {
    // Um Polygon é uma lista (ou alguma outra estrutura de dados) de pontos
    Structure * vertices;
    char * (*to_string)(const struct polygon *);
} Polygon;

void * create_point(Point *);
void * create_line(Point *);
void * create_polygon(Point *);

#endif