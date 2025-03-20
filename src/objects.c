#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GL/glut.h>
#include "objects.h"
#include "utils.h"

#define POINT_BUFFER_SIZE 16
#define LINE_BUFFER_SIZE 2 * (POINT_BUFFER_SIZE - 1) + 5
#define SIZE 2

static bool creating_line = false;
static bool creating_polygon = false;
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

void draw_point(Point * self) {
    printf(BLUE "Debug: Desenhando ponto\n\n" RESET);
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(SIZE);
    glBegin(GL_POINTS);
        glVertex2i(((Point*)self)->x, ((Point*)self)->y);
    glEnd();
}

void draw_line(Line * self) {
    printf(BLUE "Debug: Desenhando Linha\n\n" RESET);
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(SIZE);
    glBegin(GL_LINES);
        glVertex2i(self->s_point->x, self->s_point->y);
        glVertex2i(self->e_point->x, self->e_point->y);
    glEnd();
}

void draw_polygon(Polygon * self) {
    printf(BLUE "Debug: Desenhando Polígono\n\n" RESET);

    int num_objects = get_num_objects(self->vertices);
    Node_ptr *objects_list = get_all(self->vertices);

    if (num_objects < 3) {
        printf(BLUE "\nDebug: Adicione mais %d vértices.\n\n" RESET, 3 - num_objects);
        return;
    }

    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(SIZE);

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num_objects; i++) {
        Point *p = (Point *)objects_list[i]->object;
        glVertex2i(p->x, p->y);
    }
    glEnd();
}

void * object_factory(const Object data, const Objec_t type) {
    switch (type) {
        case POINT:
            Point * new_point = malloc(sizeof(Point));
            verify_allocation_error(new_point);
            new_point->x = ((Point*)data)->x;
            new_point->y = ((Point*)data)->y;
            new_point->to_string = point_to_string;
            new_point->draw = draw_point;
            return new_point;

        case LINE:
            Line * new_line = malloc(sizeof(Line));
            verify_allocation_error(new_line);
            new_line->e_point = ((Line*)data)->e_point;
            new_line->s_point = ((Line*)data)->s_point;
            new_line->to_string = line_to_string;
            new_line->draw = draw_line;
            return new_line;

        case POLYGON:
            Polygon * new_polygon = malloc(sizeof(Polygon));
            verify_allocation_error(new_polygon);
            new_polygon->vertices = (Structure*)data;
            new_polygon->to_string = polygon_to_string;
            new_polygon->draw = draw_polygon;
            return new_polygon;
        
        default:
            perror(RED "Error: Non-existent type" RESET);
            return 0;
    }
}

void disable_state() {
    creating_polygon = false;
    creating_line = false;
}

void * create_point(Point * p) {
    if (p == NULL) return NULL;
    creating_polygon = false;
    creating_line = false;
    return object_factory(p, POINT);
}

void * create_line(Point * p) {
    creating_line = !creating_line;
    creating_polygon = false;

    if (creating_line) {
        line_aux = malloc(sizeof(Line));
        verify_allocation_error(line_aux);

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
        add_object(polygon_vertices_aux, p, POINT);
        return object_factory(polygon_vertices_aux, POLYGON);
    }

    add_object(polygon_vertices_aux, p, POINT);
    return NULL;
}