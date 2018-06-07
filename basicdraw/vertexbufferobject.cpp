#include <stdlib.h>
#include "glew.h"
#include "glut.h"

GLuint vboId;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);

    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

    glFlush();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Vertex Buffer Object");

    glewInit();//vertex buffer needed

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(0.0, 1.0, 1.0);

    GLfloat vertices[] = {
        -0.5, -0.5,
        -0.5, 0.5,
        0.5, -0.5
    };

    glGenBuffersARB(1, &vboId);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices), vertices, GL_STATIC_DRAW_ARB);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}
