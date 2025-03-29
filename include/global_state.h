#ifndef GLOBAL_STATE_H
#define GLOBAL_STATE_H

#include "objects.h"

typedef struct global_state
{
    Node * selected_node;
    Structure * all_nodes;
    bool creating_line;
    bool creating_polygon;
} GlobalState;

void init_global_state();
void add_selected_node(Node *);
void g_add_object(Object, Objec_t);
Node_ptr g_get_head();
void g_set_creating_line(bool);
void g_set_creating_polygon(bool);
bool g_get_creating_line();
bool g_get_creating_polygon();

#endif