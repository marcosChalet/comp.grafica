#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"
#include "transformations.h"
#include "opengl_functions.h"

#define ANGLE 30
#define M_PI acos(-1.0)

void rotate_point(Point_d * p, Point_d * origin) {
    double radians = ANGLE * (M_PI / 180.0);
    double x_shifted = p->x - origin->x;
    double y_shifted = p->y - origin->y;
    
    double x_new = x_shifted * cos(radians) - y_shifted * sin(radians);
    double y_new = x_shifted * sin(radians) + y_shifted * cos(radians);
    
    p->x = x_new + origin->x;
    p->y = y_new + origin->y;
}

void rotate_line(Line_d * l) {
    Point_d mid;
    mid.x = (l->s_point->x + l->e_point->x) / 2.0;
    mid.y = (l->s_point->y + l->e_point->y) / 2.0;
    rotate_point(l->e_point, &mid);
    rotate_point(l->s_point, &mid);
}

void rotate_polygon(Polygon_d * p) {
    Point_d centroid = {0, 0};

    Node_ptr * all_points = get_all(p->vertices);

    for (int i = 0; i < p->vertices->num_objects; i++) {
        centroid.x += ((Point_d*)all_points[i]->object)->x;
        centroid.y += ((Point_d*)all_points[i]->object)->y;
    }

    centroid.x /= p->vertices->num_objects;
    centroid.y /= p->vertices->num_objects;

    for (int i = 0; i < p->vertices->num_objects; i++) {
        rotate_point((Point_d*)all_points[i]->object, &centroid);
    }
}

void * rotate(Object o) {

    if (o == NULL || ((Node_ptr)o)->object) {
        perror(RED "ERROR: O objeto não foi selecionado\n" RESET);
        return NULL;
    }

    glut_post_redisplay();
    switch (((Node_ptr)o)->type) {
        case LINE_T:
            rotate_line(((Node_ptr)o)->object);
            break;
        case POLYGON_T:
            rotate_polygon(((Node_ptr)o)->object);
            break;
        default:
            break;
    }

    return NULL;
}