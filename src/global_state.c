#include <stdlib.h>
#include "global_state.h"

GlobalState *global_state;

void init_global_state() {
    global_state = (GlobalState *) malloc(sizeof(GlobalState));

    global_state->selected_node = NULL;
    global_state->all_nodes = create_structure();
    global_state->creating_line = global_state->creating_polygon = false;
}

void add_selected_node(Node *node) {
    global_state->selected_node = node;
}

void g_add_object(Object object, Objec_t type) {
    add_object(global_state->all_nodes, object, type);
}

Node_ptr g_get_head() {
    return global_state->all_nodes->head;
}

void g_set_creating_line(bool new_state){
    global_state->creating_line = new_state;
}

void g_set_creating_polygon(bool new_state){
    global_state->creating_polygon = new_state;
}

bool g_get_creating_line(){
    return global_state->creating_line;
}

bool g_get_creating_polygon(){
    return global_state->creating_polygon;
}
