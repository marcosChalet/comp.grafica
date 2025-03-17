#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "utils.h"
#include "objects.h"
#include "objects_data_structure.h"

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

    /**
     * Template de execução! posteriormente será implementado
     * o projeto do paint (definição de tela, etc).
     */

    /* Exemplo de ponto */
    Point* newPoint1 = malloc(sizeof(Point));
    Point* newPoint2 = malloc(sizeof(Point));
    newPoint1->x = 3; newPoint1->y = 5;
    newPoint2->x = 7; newPoint2->y = 9;

    Point* p1 = event_keyboard(newPoint1, 'p');
    Point* p2 = event_keyboard(newPoint2, 'p');

    // Deletar elemento / obj é a estrutura de dados quem tem esse elemento
    // A sobrecarga já está implementada
    // Point* p3 = event_keyboard(obj, 'd', 2);


    /* Exemplo de linha */
    Line* l1;
    l1 = event_keyboard(p1, 'l');
    l1 = event_keyboard(p2, 'l');

    Line* l2;
    l2 = event_keyboard(p2, 'l');
    l2 = event_keyboard(p1, 'l');
    

    /* Exemplo de polígono */
    Polygon * plg1 = event_keyboard(p1, 'g');
    event_keyboard(p2, 'g');
    event_keyboard(p2, 'g');
    event_keyboard(p2, 'g');
    event_keyboard(p1, 'g');
    event_keyboard(NULL, 'g');

    Polygon * plg2 = event_keyboard(p2, 'g');
    event_keyboard(p1, 'g');
    event_keyboard(p1, 'g');
    event_keyboard(p1, 'g');
    event_keyboard(p2, 'g');
    event_keyboard(NULL, 'g');


    /* Mostrando resultado */
    printf("p1: %s\n", to_string(p1));
    printf("p2: %s\n", to_string(p2));
    printf("l1: %s\n", to_string(l1));
    printf("l2: %s\n", to_string(l2));
    printf("plg1: %s\n", to_string(plg1));
    printf("plg2: %s\n", to_string(plg2));

    /* Testando OpenGL */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(200, 0);
    glutCreateWindow("Triangulo em OpenGL");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}