#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include "objects.h"
#include "utils.h"

#define POINT_BUFFER_SIZE 7
#define LINE_BUFFER_SIZE 2 * (POINT_BUFFER_SIZE - 1) + 5

static bool creating_line = false;
static bool creating_polygon = false;
static Line * line_aux;
static Structure * polygon_vertices_aux;

char * point_to_string(const Point * p) {
    static char buffer[POINT_BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "{%d, %d}", p->x, p->y);
    return buffer;
}

char * line_to_string(const Line * l) {
    static char buffer[LINE_BUFFER_SIZE];
    char point1[POINT_BUFFER_SIZE];
    char point2[POINT_BUFFER_SIZE];

    snprintf(point1, sizeof(point1), "%s", point_to_string(l->e_point));
    snprintf(point2, sizeof(point2), "%s", point_to_string(l->s_point));

    snprintf(buffer, sizeof(buffer), "[%s, %s]", point1, point2);
    return buffer;
}

char * polygon_to_string(const Polygon *plg) {
    if (plg == NULL) return NULL;
    if (plg->vertices == NULL) return NULL;
    if (plg->vertices->head == NULL) return NULL;

    size_t num_vertices = get_num_objects(plg->vertices);
    size_t total_size = num_vertices * (POINT_BUFFER_SIZE - 1) + (num_vertices > 0 ? (num_vertices - 1) * 2 : 0) + 3;
    
    char *buffer = malloc(total_size);
    veirfy_allocation_error(buffer);

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
        case POINT:
            Point * new_point = malloc(sizeof(Point));
            veirfy_allocation_error(new_point);
            new_point->x = ((Point*)data)->x;
            new_point->y = ((Point*)data)->y;
            new_point->to_string = point_to_string;
            return new_point;

        case LINE:
            Line * new_line = malloc(sizeof(Line));
            veirfy_allocation_error(new_line);
            new_line->e_point = ((Line*)data)->e_point;
            new_line->s_point = ((Line*)data)->s_point;
            new_line->to_string = line_to_string;
            return new_line;

        case POLYGON:
            Polygon * new_polygon = malloc(sizeof(Polygon));
            veirfy_allocation_error(new_polygon);
            new_polygon->vertices = (Structure*)data;
            new_polygon->to_string = polygon_to_string;
            return new_polygon;
        
        default:
            perror("Non-existent type");
            return 0;
    }
}

void * create_point(Point * p) {
    if (p == NULL) return NULL;
    return object_factory(p, POINT);
}

void * create_line(Point * p) {
    creating_line = !creating_line;
    creating_polygon = false;

    if (creating_line) {
        line_aux = malloc(sizeof(Line));
        veirfy_allocation_error(line_aux);

        line_aux->e_point = p;
        return NULL;
    }

    line_aux->s_point = p;
    return object_factory(line_aux, LINE);
}

void * create_polygon(Point * p) {
    if (creating_polygon == true && p == NULL) {
        creating_polygon = false;
        return NULL;
    }

    if (creating_polygon == false) {
        creating_polygon = true;
        polygon_vertices_aux = create_structure();
        add_object(polygon_vertices_aux, p);
        return object_factory(polygon_vertices_aux, POLYGON);
    }

    add_object(polygon_vertices_aux, p);
    return NULL;
}