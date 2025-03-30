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
    SCALE_UP = '+',
    SCALE_DOWN = '-',
    REFLECT = 'x',
    SHEAR = 'c',
    VIEW_MODE = 27,
} Keyboard_Key_t;

void draw_objects();
void handle_mouse_event(int, int, int, int);
void handle_keyboard_event_special(int, int, int);
void handle_keyboard_event(unsigned char, int, int);

#endif