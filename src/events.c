#include <stdio.h>
#include <GL/glut.h>
#include "objects.h"
#include "events.h"
#include "utils.h"

Structure * objects;
static Keyboard_Key_t mode = VIEW_MODE;

void * change_to_view_mode() {
    event_keyboard(NULL, mode);
    mode = VIEW_MODE;
    return NULL;
}

void * process_event(Object p, Keyboard_Key_t event_key) {
    switch (event_key) {
        case CREATING_POINT   : return create_point(p);
        case CREATING_LINE    : return create_line(p);
        case CREATING_POLYGON : return create_polygon(p);
        case VIEW_MODE        : return change_to_view_mode();
        case DELETE_OBJECT    : return NULL;
        case ROTATE           : return NULL;
     // case ...              : ...
     // case ...              : ...
     // case ...              : ...
        default               : return NULL;
    }
}

void handle_keyboard_event(unsigned char key, int x, int y) {
    mode = key;
}

void handle_keyboard_event_special(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F10   : exit(0);
        case GLUT_KEY_F12   : break; // fullscreen
        case GLUT_KEY_LEFT  : break;
        case GLUT_KEY_RIGHT : break;
        case GLUT_KEY_UP    : break;
        case GLUT_KEY_DOWN  : break;
    }
}

void handle_mouse_event(int button, int state, int x, int y) {
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    int convertedY = windowHeight - y;

    Point * p = malloc(sizeof(Point));
    veirfy_allocation_error(p);
    
    p->x = x;
    p->y = windowHeight - y;
    Object obj = process_event(p, mode);
    if (obj == NULL) return;

    add_object(objects, obj);
}