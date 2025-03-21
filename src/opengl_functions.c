#include <stdio.h>
#include <GL/glut.h>
#include "opengl_functions.h"
#include "utils.h"

#define SIZE 2

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

    glColor3f(0.9, 0.9, 0.9);
    glLineWidth(SIZE);

    glBegin(GL_POLYGON);
    for (int i = 0; i < num_objects; i++) {
        Point *p = (Point *)objects_list[i]->object;
        glVertex2i(p->x, p->y);
    }
    glEnd();

    glColor3f(0.4, 0.4, 0.4);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num_objects; i++) {
        Point *p = (Point *)objects_list[i]->object;
        glVertex2i(p->x, p->y);
    }
    glEnd();
}

void glut_post_redisplay() {
    glutPostRedisplay();
}
