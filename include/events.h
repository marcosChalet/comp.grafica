#ifndef EVENTS_H
#define EVENTS_H

#include "objects.h" 

typedef enum {
    CREATING_POINT = 'p',
    CREATING_LINE = 'l',
    CREATING_POLYGON = 'g',
    DELETE_OBJECT = 8,
    ROTATE = 'r',
    SELECT = 's',
    TRANSLATE = 't',
    SCALE_UP = '1',
    SCALE_UP_X = '2',
    SCALE_UP_Y = '3',
    SCALE_DOWN = '4',
    SCALE_DOWN_X = '5',
    SCALE_DOWN_Y = '6',
    SHEAR = '7',
    SHEAR_X = '8',
    SHEAR_Y = '9',
    TO_CONVEX = '0',
    REFLECT = 'x',
    VIEW_MODE = 27,
} Keyboard_Key_t;

void draw_objects();
void handle_mouse_event(int, int, int, int);
void handle_keyboard_event_special(int, int, int);
void handle_keyboard_event(unsigned char, int, int);

#endif