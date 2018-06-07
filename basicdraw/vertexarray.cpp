#include <stdlib.h>
#include "glew.h"
#include "glut.h"

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    GLfloat vertices[] = {
        -0.5, -0.5,
        0, 0.5,
        0.5, -0.5
    };

    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, vertices);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableClientState(GL_VERTEX_ARRAY);

    glFlush();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Vertex Array");

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(0.0, 1.0, 1.0);

    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}
