#include <stdlib.h>
#include "glew.h"
#include "glut.h"

GLuint l;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glCallList(l);

    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 033:
        exit(EXIT_SUCCESS);
        break;
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Call List");

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(1.0, 0.0, 1.0);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    l = glGenLists(1);
    glNewList(l, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5, -0.5);
    glVertex2f(0, 0.5);
    glVertex2f(0.5, -0.5);
    glEnd();
    glEndList();

    glutMainLoop();

    return 0;
}
