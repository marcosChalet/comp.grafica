#include <stdio.h>
#include <GL/glut.h>
#include "database.h"
#include "objects.h"
#include "events.h"
#include "utils.h"

Structure objects;
static Keyboard_Key_t mode = VIEW_MODE;
extern int windowHeight;

void * process_event(Object, Keyboard_Key_t);

void draw_objects() {
    int num_objects = get_num_objects(&objects);
    Node_ptr * objects_list = get_all(&objects);
    for (int i = 0; objects_list[i] != NULL; i++) {
        switch (objects_list[i]->type)
        {
            case POINT_T: draw(((Point*)objects_list[i]->object)); break;
            case LINE_T: draw(((Line*)objects_list[i]->object)); break;
            case POLYGON_T: draw(((Polygon*)objects_list[i]->object)); break;
            default: break;
        }
    }

    printf("Teste\n");
}

void * change_to_view_mode() {
    disable_state();
    return NULL;
}

void * process_event(Object p, Keyboard_Key_t event_key) {
    glutPostRedisplay();
    switch (event_key) {
        case CREATING_POINT   : return create_point(p);
        case CREATING_LINE    : return create_line(p);
        case CREATING_POLYGON : return create_polygon(p);
        case VIEW_MODE        : return change_to_view_mode();
        case DELETE_OBJECT    : return NULL;
        case ROTATE           : return NULL;
        case SELECT           : return handle_select_object(p);
        case TRANSLATE        : return NULL;
        case SCALE_UP         : return NULL;
        case SCALE_DOWN       : return NULL;
        case REFLECT          : return NULL;
        case SHEAR            : return NULL;
        default               : return NULL;
    }
}

void handle_keyboard_event(unsigned char key, int x, int y) {
    if (mode == key) {
        change_to_view_mode();
        mode = VIEW_MODE;
        return;
    }
    mode = key;
}

void handle_keyboard_event_special(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F8    : save_objects(&objects); break;
        case GLUT_KEY_F9    : load_objects(&objects); break;
        case GLUT_KEY_F10   : exit(0);
        case GLUT_KEY_F12   : break; // fullscreen
        case GLUT_KEY_LEFT  : break;
        case GLUT_KEY_RIGHT : break;
        case GLUT_KEY_UP    : break;
        case GLUT_KEY_DOWN  : break;
    }
}

void handle_mouse_event(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;

    Point * p = malloc(sizeof(Point));
    verify_allocation_error(p);
    
    p->x = x;
    p->y = windowHeight - y;
    Object obj = process_event(p, mode);
    if (obj == NULL) return;

    add_object(&objects, obj, mode_to_type(mode));
}