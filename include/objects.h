#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdarg.h>
#include "objects_data_structure.h"

typedef enum {
    POINT,
    LINE,
    POLYGON,
} Objec_t;

typedef enum {
    CREATING_POINT = 'p',
    CREATING_LINE = 'l',
    CREATING_POLYGON = 'g',
    DELETE_OBJECT = 'd',
    ROTATE = 'r',
} Keyboard_Key_t;

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

void * event_keyboard(Object, Keyboard_Key_t, int);
#define event_keyboard(...) GET_MACRO(__VA_ARGS__, event_keyboard_3, event_keyboard_2)(__VA_ARGS__)
#define GET_MACRO(_1, _2, _3, NAME, ...) NAME
#define event_keyboard_2(p, key) event_keyboard(p, key, -1)
#define event_keyboard_3(p, key, id) event_keyboard(p, key, id)

#endif