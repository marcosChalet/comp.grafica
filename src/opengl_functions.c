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
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_POLYGON_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glColor4f(0.6, 0.6, 0.9, 0.2);
    glLineWidth(SIZE);

    glBegin(GL_POLYGON);
    for (int i = 0; i < num_objects; i++) {
        Point_d *p = (Point_d *)objects_list[i]->object;
        glVertex2i(p->x, p->y);
    }
    glEnd();

    glDisable(GL_POLYGON_SMOOTH);

    // glColor4f(0.4, 0.4, 0.4, 1.0);
    // glBegin(GL_LINE_LOOP);
    // for (int i = 0; i < num_objects; i++) {
    //     Point_d *p = (Point_d *)objects_list[i]->object;
    //     glVertex2i(p->x, p->y);
    // }
    // glEnd();

    glPointSize(5.0);
    glColor3f(1.0, 0.0, 0.0);
    
    glBegin(GL_POINTS);
    for (int i = 0; i < num_objects; i++) {
        Point_d *p = (Point_d *)objects_list[i]->object;
        glVertex2f((float)p->x, (float)p->y);
    }
    glEnd();
}

void glut_post_redisplay() {
    glutPostRedisplay();
}
