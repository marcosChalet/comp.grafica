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
    veirfy_allocation_error(new_list);
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->num_items = 0;
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

bool add_object(Structure * list, const Object myObject) {
    if (is_empty(list)) {
        list = create_structure();
    }

    Node_ptr newNode = malloc(sizeof(Node));
    veirfy_allocation_error(newNode);

    newNode->id = generate_id();
    newNode->object = myObject;
    newNode->next = NULL;
    if (list->head == NULL) {
        list->head = newNode;
        list->tail = list->head;
        list->num_items = 1;
        return true;
    }

    list->tail->next = newNode;
    list->tail = newNode;
    list->num_items++;

    return true;
}

int get_num_items(Structure * list) {
    return list->num_items;
}

Node_ptr get_first(Structure * list) {
    return list->head;
}

Node_ptr get_last(Structure * list) {
    return list->tail;
}

Object get_item(Structure * list, int id) {
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
    Node_ptr * all_items = malloc(sizeof(Node_ptr) * (list->num_items + 1));
    veirfy_allocation_error(all_items);

    Node_ptr aux = list->head;
    for(int i = 0; i < list->num_items; i++) {
        all_items[i] = aux;
        aux = aux->next;
    }

    all_items[list->num_items] = NULL;

    return all_items;
}