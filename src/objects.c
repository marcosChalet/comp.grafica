#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include "objects.h"
#include "global_state.h"
#include "opengl_functions.h"
#include "utils.h"

#define POINT_BUFFER_SIZE 16
#define LINE_BUFFER_SIZE 2 * (POINT_BUFFER_SIZE - 1) + 5

static Line * line_aux;
static Structure * polygon_vertices_aux;

char * point_to_string(Point * p) {
    static char buffer[POINT_BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "{%d, %d}", p->x, p->y);
    return buffer;
}

char * line_to_string(Line * l) {
    static char buffer[LINE_BUFFER_SIZE];
    char point1[POINT_BUFFER_SIZE];
    char point2[POINT_BUFFER_SIZE];

    snprintf(point1, sizeof(point1), "%s", point_to_string(l->e_point));
    snprintf(point2, sizeof(point2), "%s", point_to_string(l->s_point));

    snprintf(buffer, sizeof(buffer), "[%s, %s]", point1, point2);
    return buffer;
}

char * polygon_to_string(Polygon * plg) {
    if (plg == NULL) return NULL;
    if (plg->vertices == NULL) return NULL;
    if (get_first(plg->vertices) == NULL) return NULL;

    size_t num_vertices = get_num_objects(plg->vertices);
    size_t total_size = num_vertices * (POINT_BUFFER_SIZE - 1) + (num_vertices > 0 ? (num_vertices - 1) * 2 : 0) + 3;
    
    char *buffer = malloc(total_size);
    verify_allocation_error(buffer);

    size_t offset = 0;
    offset += snprintf(buffer + offset, total_size - offset, "[");

    Node_ptr * all_polgons = get_all(plg->vertices);
    for (int i = 0; all_polgons[i] != NULL; i++) {
        const char *point_str = to_string((Point*)all_polgons[i]->object);
        offset += snprintf(buffer + offset, total_size - offset, "%s", point_str);
        if (all_polgons[i+1] != NULL) {
            offset += snprintf(buffer + offset, total_size - offset, ", ");
        }
    }

    snprintf(buffer + offset, total_size - offset, "]");
    return buffer;
}

void * object_factory(const Object data, const Objec_t type) {
    switch (type) {
        case POINT_T:
            Point * new_point = malloc(sizeof(Point));
            verify_allocation_error(new_point);
            new_point->x = ((Point*)data)->x;
            new_point->y = ((Point*)data)->y;
            new_point->to_string = point_to_string;
            new_point->draw = draw_point;
            g_add_object(new_point, POINT_T);
            
            return new_point;

        case LINE_T:
            Line * new_line = malloc(sizeof(Line));
            verify_allocation_error(new_line);
            new_line->e_point = ((Line*)data)->e_point;
            new_line->s_point = ((Line*)data)->s_point;
            new_line->to_string = line_to_string;
            new_line->draw = draw_line;
            g_add_object(new_line, LINE_T);

            return new_line;

        case POLYGON_T:
            Polygon * new_polygon = malloc(sizeof(Polygon));
            verify_allocation_error(new_polygon);
            new_polygon->vertices = (Structure*)data;
            new_polygon->to_string = polygon_to_string;
            new_polygon->draw = draw_polygon;
            g_add_object(new_polygon, POLYGON_T);

            return new_polygon;
        
        default:
            perror(RED "Error: Non-existent type" RESET);
            return 0;
    }
}

void disable_state() {
    g_set_creating_polygon(false);
    g_set_creating_line(false);
}

void * create_point(Point * p) {
    if (p == NULL) return NULL;
    g_set_creating_polygon(false);
    g_set_creating_line(false);
    return object_factory(p, POINT_T);
}

void * create_line(Point * p) {
    g_set_creating_line(!g_get_creating_line());
    g_set_creating_polygon(false);

    if (g_get_creating_line()) {
        line_aux = malloc(sizeof(Line));
        verify_allocation_error(line_aux);

        line_aux->e_point = p;
        return NULL;
    }

    line_aux->s_point = p;
    return object_factory(line_aux, LINE_T);
}

void * create_polygon(Point * p) {
    if (g_get_creating_polygon() == true && p == NULL) {
        g_set_creating_polygon(false);
        return NULL;
    }

    if (g_get_creating_polygon() == false) {
        g_set_creating_polygon(true);
        polygon_vertices_aux = create_structure();
        add_object(polygon_vertices_aux, p, POINT_T);
        return object_factory(polygon_vertices_aux, POLYGON_T);
    }

    add_object(polygon_vertices_aux, p, POINT_T);
    return NULL;
}

bool check_is_selected_point(Point *m, Node_ptr node) {
    Point *p = node->object;

    if (
        m->x <= p->x + HALF_TOLERANCY &&
        m->x >= p->x - HALF_TOLERANCY &&
        m->y <= p->y + HALF_TOLERANCY &&
        m->y >= p->y - HALF_TOLERANCY
    ) {
        return true;
    }

    return false;
}

bool check_is_selected_line(Point *m, Node *node) {
    Line *l = node->object;
    int x_min = m->x - HALF_TOLERANCY,
        x_max = m->x + HALF_TOLERANCY,
        y_min = m->y - HALF_TOLERANCY,
        y_max = m->y + HALF_TOLERANCY,
        x1 = l->s_point->x,
        y1 = l->s_point->y,
        x2 = l->e_point->x,
        y2 = l->e_point->y,
        x, y;

    RegionCode
        start_point_region_code = compute_out_code(
            x1, y1, x_max, x_min, y_max, y_min),
        end_point_region_code = compute_out_code(
            x2, y2, x_max, x_min, y_max, y_min);
    
    while (true)
    {
        if (start_point_region_code == 0 && end_point_region_code == 0) {
            printf("Linha selecionada\n");
            return true; // todos os pontos estão dentro da janela
        }
        else if (start_point_region_code & end_point_region_code != 0) {
            break; // ambos os pontos estão fora da janela
        } else {
            // pelo menos um dos pontos fora da reta
            __int8_t outcode_out = start_point_region_code != 0 ?
                                    start_point_region_code :
                                    end_point_region_code;
            
            if (outcode_out & TOP) {
                x = x1 + (x2 - x1) * (y_max - y1) / (y2 - y1);
                y = y_max;
            } else if (outcode_out & BOTTOM) {
                x = x1 + (x2 - x1) * (y_min - y1) / (y2 - y1);
                y = y_min;
            } else if (outcode_out & LEFT) {
                y = y1 + (y2 - y1) * (x_max - x1) / (x2 - x1);
                x = x_max;
            } else if (outcode_out & RIGHT) {
                y = y1 + (y2 - y1) * (x_min - x1) / (x2 - x1);
                x = x_min;
            }

            if (outcode_out == start_point_region_code) {
                x1 = x;
                y1 = y;
                start_point_region_code = compute_out_code(
                    x1, y1, x_max, x_min, y_max, y_min);
            } else {
                x2 = x;
                y2 = y;
                end_point_region_code = compute_out_code(
                    x2, y2, x_max, x_min, y_max, y_min);
            }
        }
    }
    
    return false;
}

bool check_is_selected_polygon(Point *m, Node *node) {
    Polygon *polygon = (Polygon *)node->object;
    if (polygon == NULL || polygon->vertices == NULL) return false;

    int count = 0;
    size_t num_vertices = get_num_objects(polygon->vertices);
    if (num_vertices < 3) return false; // Não é um polígono válido

    Node_ptr *vertices = get_all(polygon->vertices);

    for (int i = 0; i < num_vertices; i++) {
        Point *p1 = (Point *)vertices[i]->object;
        Point *p2 = (Point *)vertices[(i + 1) % num_vertices]->object;

        // Verifica se o ponto m está dentro da tolerância de um dos vértices
        if (
            m->x <= p1->x + HALF_TOLERANCY &&
            m->x >= p1->x - HALF_TOLERANCY &&
            m->y <= p1->y + HALF_TOLERANCY &&
            m->y >= p1->y - HALF_TOLERANCY
        ) {
            return true;
        }

        // Ray casting: conta interseções com arestas
        if ((p1->y > m->y) != (p2->y > m->y)) {
            float x_intersect = (float)(p2->x - p1->x) * (m->y - p1->y) / (float)(p2->y - p1->y) + p1->x;
            if (m->x < x_intersect) {
                count++;
            }
        }
    }

    return (count % 2 == 1);
}


void *handle_select_object(Point *point) {
    // percorrer todos os objetos
    Node_ptr node = g_get_head();
    bool selected = false;

    while (true) {
        if (!node) {
            printf("Node Nil: %p\n", node);
            break;
        }
        
        printf("Node: %p\n", node);

        switch(node->type) {
            case POINT_T:
                selected = check_is_selected_point(point, node);
                break;
            case LINE_T:
                selected = check_is_selected_line(point, node);
                break;
            case POLYGON_T:
                selected = check_is_selected_polygon(point, node);
                break;
            default:
                break;
        }

        if (selected) {
            printf("selecting node\n");
            set_selected_node(node);
            return NULL;
        }
        
        node = node->next;
    }

    set_selected_node(NULL);
    return NULL;
}