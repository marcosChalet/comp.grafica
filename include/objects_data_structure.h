#ifndef OBJECTS_DATA_STRUCTURE_H
#define OBJECTS_DATA_STRUCTURE_H

#include <stdbool.h>

typedef void * Object;
typedef struct node * Node_ptr;
typedef struct node {
    Object object;
    Node_ptr next;
    Node_ptr prev;
    int id;
} Node;

typedef struct structure {
    int num_items;
    Node_ptr head;
    Node_ptr tail;
} Structure;

Structure * create_structure();
bool add_object(Structure *, const Object);
bool remove_object(Structure *, const int);
int get_num_items(Structure *);
Node_ptr get_first(Structure *);
Node_ptr get_last(Structure *);
Node_ptr * get_all(Structure *);
Object get_item(Structure *, int);

#endif