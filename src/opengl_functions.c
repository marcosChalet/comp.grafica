#include <stdio.h>
#include <GL/glut.h>
#include "opengl_functions.h"
#include "utils.h"

#define SIZE 2

void draw_point(Point_d * self) {
    printf(BLUE "DEBUG: Desenhando ponto\n" RESET);
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(SIZE*2);
    glBegin(GL_POINTS);
        glVertex2i(((Point_d*)self)->x, ((Point_d*)self)->y);
    glEnd();
}

void draw_line(Line_d * self) {
    printf(BLUE "DEBUG: Desenhando Linha\n" RESET);
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(SIZE);
    glBegin(GL_LINES);
        glVertex2i(self->s_point->x, self->s_point->y);
        glVertex2i(self->e_point->x, self->e_point->y);
    glEnd();
}

void draw_polygon(Polygon_d * self) {
    printf(BLUE "DEBUG: Desenhando Polígono\n" RESET);

    int num_objects = get_num_objects(self->vertices);
    Node_ptr *objects_list = get_all(self->vertices);

    if (num_objects < 3) {
        printf(BLUE "\nDEBUG: Adicione mais %d vértices.\n" RESET, 3 - num_objects);
        return;
    }

    glColor3f(0.9, 0.9, 0.9);
    glLineWidth(SIZE);

    glBegin(GL_POLYGON);
    for (int i = 0; i < num_objects; i++) {
        Point_d *p = (Point_d *)objects_list[i]->object;
        glVertex2i(p->x, p->y);
    }
    glEnd();

    glColor3f(0.4, 0.4, 0.4);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num_objects; i++) {
        Point_d *p = (Point_d *)objects_list[i]->object;
        glVertex2i(p->x, p->y);
    }
    glEnd();
}

void glut_post_redisplay() {
    glutPostRedisplay();
}
