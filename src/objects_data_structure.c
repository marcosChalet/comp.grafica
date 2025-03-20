#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "objects_data_structure.h"

int generate_id() {
    static int id = 0;
    return id++;
}

Structure * create_structure() {
    Structure * new_list = malloc(sizeof(Structure));
    verify_allocation_error(new_list);
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->num_objects = 0;
    return new_list;
}

bool is_empty(const Object obj) {
    return (obj == NULL);
}

bool remove_object(Structure * list, const int id) {
    if (is_empty(list->head)) return false;

    Node_ptr aux = list->head;
    Node_ptr prev = NULL;
    if (list->head->id == id) {
        list->head = list->head->next;
        free(aux);
        return true;
    }

    while(aux && aux->id != id) {
        prev = aux;
        aux = aux->next;
    }
    
    if (aux == NULL) return false;
    prev->next = aux->next;
    free(aux);

    return true;
}

bool add_object(Structure * list, const Object my_object, int type) {
    if (is_empty(list)) {
        list = create_structure();
    }

    Node_ptr new_node = malloc(sizeof(Node));
    verify_allocation_error(new_node);

    new_node->id = generate_id();
    new_node->object = my_object;
    new_node->next = NULL;
    new_node->type = type;

    if (list->head == NULL) {
        list->head = new_node;
        list->tail = list->head;
        list->num_objects = 1;
        return true;
    }

    list->tail->next = new_node;
    list->tail = new_node;
    list->num_objects++;

    return true;
}

int get_num_objects(Structure * list) {
    return list->num_objects;
}

Node_ptr get_first(Structure * list) {
    return list->head;
}

Node_ptr get_last(Structure * list) {
    return list->tail;
}

Object get_object(Structure * list, int id) {
    Node_ptr aux = list->head;
    while(aux->next) {
        if (aux->id == id) {
            return aux->object;
        }
        aux = aux->next;
    }
    return NULL;
}

Node_ptr * get_all(Structure * list) {
    
    if (!list) return NULL;

    Node_ptr * all_objects = malloc(sizeof(Node_ptr) * (list->num_objects + 1));
    verify_allocation_error(all_objects);

    Node_ptr aux = list->head;
    for(int i = 0; i < list->num_objects; i++) {
        all_objects[i] = aux;
        aux = aux->next;
    }

    all_objects[list->num_objects] = NULL;

    return all_objects;
}