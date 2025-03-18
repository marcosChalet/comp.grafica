#ifndef EVENTS_H
#define EVENTS_H

#include "objects.h" 

typedef enum {
    CREATING_POINT = 'p',
    CREATING_LINE = 'l',
    CREATING_POLYGON = 'g',
    DELETE_OBJECT = 'd',
    ROTATE = 'r',
    VIEW_MODE = 27,
} Keyboard_Key_t;

void mouse_event(int, int, int, int);
void keyboard_event_special(int, int, int);
void keyboard_event(unsigned char, int, int);
void * event_keyboard(Object, Keyboard_Key_t);

#endif