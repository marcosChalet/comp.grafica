#include <stdio.h>
#include <GL/glut.h>
#include "transformations.h"
#include "global_state.h"
#include "database.h"
#include "objects.h"
#include "events.h"
#include "utils.h"

static Keyboard_Key_t mode = VIEW_MODE;
extern int windowHeight;

void * process_event(Object, Keyboard_Key_t);

void draw_objects() {
    int num_objects = get_num_objects(g_get_structure());
    Node_ptr * objects_list = get_all(g_get_structure());
    for (int i = 0; objects_list[i] != NULL; i++) {
        switch (objects_list[i]->type)
        {
            case POINT_T: draw(((Point_d*)objects_list[i]->object)); break;
            case LINE_T: draw(((Line_d*)objects_list[i]->object)); break;
            case POLYGON_T: draw(((Polygon_d*)objects_list[i]->object)); break;
            default: break;
        }
    }
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
        case SELECT           : return handle_select_object(p);
        case TRANSLATE        : return translate(get_selected_node(), p);
        default               : return NULL;
    }
}

void remove_fn() {
    Node_ptr selected_node = get_selected_node();
    if (selected_node == NULL) return;
    remove_object(g_get_structure(), selected_node->id);
    glutPostRedisplay();
}

void handle_keyboard_event(unsigned char key, int x, int y) {
    if (mode == key) {
        change_to_view_mode();
        mode = VIEW_MODE;
        return;
    }
    
    mode = key;

    char * str_mode = enum_to_string(mode);
    if (str_mode != NULL) printf(GREEN "LOG: Entrando no modo %s\n" RESET, str_mode);
    else printf(RED "ERROR: Opção inválida\n" RESET);
    free(str_mode);

    switch (mode) {
        case VIEW_MODE        : change_to_view_mode(); break;
        case DELETE_OBJECT    : remove_fn(); break;
        case ROTATE           : rotate(get_selected_node()); break;
        case SCALE_UP         : scale(get_selected_node(), true, true, true); break;
        case SCALE_UP_X       : scale(get_selected_node(), true, true, false); break;
        case SCALE_UP_Y       : scale(get_selected_node(), true, false, true); break;
        case SCALE_DOWN       : scale(get_selected_node(), false, true, true); break;
        case SCALE_DOWN_X     : scale(get_selected_node(), false, true, false); break;
        case SCALE_DOWN_Y     : scale(get_selected_node(), false, false, true); break;
        // case REFLECT          : break;
        case SHEAR            : handle_shear_object();
        default               : break;
    }

    glutPostRedisplay();
}

void handle_keyboard_event_special(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F8    : save_objects(g_get_structure()); break;
        case GLUT_KEY_F9    : load_objects(g_get_structure()); break;
        case GLUT_KEY_F10   : exit(0);
        case GLUT_KEY_F12   : break; // fullscreen
        case GLUT_KEY_LEFT  : reflect(get_selected_node(), false); break;
        case GLUT_KEY_RIGHT : reflect(get_selected_node(), false); break;
        case GLUT_KEY_UP    : reflect(get_selected_node(), true); break;
        case GLUT_KEY_DOWN  : reflect(get_selected_node(), true); break;
    }
}

void handle_mouse_event(int button, int state, int x, int y) {
    if (mode != SELECT && state != GLUT_DOWN) {
        return;
    }

    if (button != GLUT_LEFT_BUTTON) {
        return;
    }

    if (mode == TRANSLATE) {
        mode = SELECT;
    }

    if (state == GLUT_UP && mode == SELECT) {
        mode = TRANSLATE;
    }

    Point_d * p = malloc(sizeof(Point_d));
    verify_allocation_error(p);
    
    p->x = x;
    p->y = windowHeight - y;
    Object obj = process_event(p, mode);
    if (obj == NULL) return;
}