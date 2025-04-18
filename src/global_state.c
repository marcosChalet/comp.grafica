#include <stdlib.h>
#include "global_state.h"

GlobalState *global_state;

void init_global_state() {
    global_state = (GlobalState *) malloc(sizeof(GlobalState));

    global_state->selected_node = NULL;
    global_state->all_nodes = create_structure();
    global_state->creating_line = global_state->creating_polygon = false;
}

void set_selected_node(Node *node) {
    global_state->selected_node = node;
}

Node * get_selected_node() {
    if (!global_state) return NULL;
    if (!global_state->selected_node) return NULL;
    return global_state->selected_node;
}

void g_add_object(Object object, Objec_t type) {
    add_object(global_state->all_nodes, object, type);
}

Structure * g_get_structure() {
    return global_state->all_nodes;
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