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
} Point_d;

typedef struct line {
    Point_d * s_point;
    Point_d * e_point;
    void (*draw)(struct line *);
    char * (*to_string)(struct line *);
} Line_d;

typedef struct polygon {
    Structure * vertices;
    void (*draw)(struct polygon *);
    char * (*to_string)(struct polygon *);
} Polygon_d;

void disable_state();
void * create_point(Point_d *);
void * create_line(Point_d *);
void * create_polygon(Point_d *);
void * object_factory(const Object, const Objec_t);
void * handle_select_object(Point_d *);
bool check_is_selected_point(Point_d *, Node *);
bool check_is_selected_line(Point_d *, Node *);
bool check_is_selected_polygon(Point_d *, Node *);

#endif