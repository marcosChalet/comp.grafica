#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "events.h"
#include "global_state.h"
#ifdef _WIN32
    #include <windows.h>
#endif

#define SCREEN_SCALE 0.5

int windowWidth, windowHeight;

void set_window_size() {
    windowWidth = glutGet(GLUT_SCREEN_WIDTH) * SCREEN_SCALE;
    windowHeight = glutGet(GLUT_SCREEN_HEIGHT) * SCREEN_SCALE;
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    draw_objects();
    glutSwapBuffers();
}

void init(void) {
    init_global_state();
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
}

void reshape(int width, int height) {
    if (height == 0) height = 1;

    windowWidth = width;
    windowHeight = height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0.0, width, 0.0, height);

    glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    set_window_size();
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - windowWidth) / 2, 
                           (glutGet(GLUT_SCREEN_HEIGHT) - windowHeight) / 2);
    glutCreateWindow("PAINT");

    init();

    // glutFullScreen();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(handle_keyboard_event);
    glutSpecialFunc(handle_keyboard_event_special);
    glutMouseFunc(handle_mouse_event);

    glutMainLoop();
    return 0;
}
