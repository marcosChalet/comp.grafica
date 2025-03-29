#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdarg.h>
#include "objects_data_structure.h"

#define HALF_TOLERANCY 10

typedef enum {
    POINT_T,
    LINE_T,
    POLYGON_T,
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

void disable_state();
void * create_point(Point *);
void * create_line(Point *);
void * create_polygon(Point *);
void * object_factory(const Object, const Objec_t);
void * handle_select_object(Point *);
bool check_is_selected_point(Point *, Point *);
bool check_is_selected_line(Point *, Line *);
bool check_is_selected_polygon(Point *);

#endif