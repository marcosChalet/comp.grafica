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

bool remove_object(Structure *list, const int id) {
    if (!list || is_empty(list)) return false;

    Node_ptr aux = list->head;
    int count = 0;

    while (count < list->num_objects && aux->id != id) {
        aux = aux->next;
        count++;
    }
    if (count == list->num_objects) {
        return false;
    }

    if (list->num_objects == 1) {
        free(aux);
        list->head = list->tail = NULL;
        list->num_objects = 0;
        return true;
    }

    // Reajustar ponteiros dos vizinhos
    aux->prev->next = aux->next;
    aux->next->prev = aux->prev;

    // Atualizar head ou tail, se for o caso
    if (aux == list->head) {
        list->head = aux->next;
    }
    if (aux == list->tail) {
        list->tail = aux->prev;
    }

    list->num_objects--;

    // free_object_contents(&aux->object);
    // free(aux);
    return true;
}

bool add_object(Structure *list, const Object my_object, int type) {
    if (list == NULL) {
        return false;
    }

    Node_ptr new_node = malloc(sizeof(Node));
    verify_allocation_error(new_node);

    new_node->id = generate_id();
    new_node->object = my_object;
    new_node->type = type;

    if (list->head == NULL) {
        new_node->next = new_node;
        new_node->prev = new_node;
        list->head = new_node;
        list->tail = new_node;
        list->num_objects = 1;
    } else {
        new_node->prev = list->tail;
        new_node->next = list->head;
        
        list->tail->next = new_node;
        list->head->prev = new_node;
        
        list->tail = new_node;
        list->num_objects++;
    }

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

Structure **split_list(Structure *original, int split_index) {
    Structure **result = malloc(2 * sizeof(Structure *));
    
    result[0] = create_structure();
    result[0]->head = original->head;
    Node_ptr current = original->head;
    for(int i = 0; i < split_index-1; i++) current = current->next;
    
    Node_ptr first_tail = current;
    Node_ptr second_head = current->next;
    
    first_tail->next = result[0]->head;
    result[0]->head->prev = first_tail;
    
    result[1] = create_structure();
    result[1]->head = second_head;
    Node_ptr second_tail = original->head->prev; 
    
    second_tail->next = second_head;
    second_head->prev = second_tail;
    
    return result;
}

Structure *copy_structure(Structure *original) {
    if (!original || !original->head) return NULL;

    Structure *copy = create_structure();
    Node_ptr current = original->head;

    do {
        Object copied_object = current->object;
        add_object(copy, copied_object, current->type);
        current = current->next;
    } while (current != original->head);

    return copy;
}