#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "events.h"

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2i(50, 20);

        glColor3f(0.0, 1.0, 0.0);
        glVertex2i(150, 20);

        glColor3f(0.0, 0.0, 1.0);
        glVertex2i(100, 120);
    glEnd();

    glFlush();
}

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(200, 0);
    glutCreateWindow("PAINT");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(handle_keyboard_event);
    glutSpecialFunc(handle_keyboard_event_special);
    glutMouseFunc(handle_mouse_event);
    glutMainLoop();
    return 0;
}