#include <cstdlib>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

const char* windowTitlePrefix = "Chapter 1";

int currentWidth = 800;
int currentHeight = 600;
int windowHandle = 0;

void initialize(int, char*[]);
void initWindow(int, char*[]);
void resize(int, int);
void render();

int main (int argc, char* argv[]) {
    initialize(argc, argv);
    glutMainLoop();
    std::exit(EXIT_SUCCESS);
}

void initialize(int argc, char* argv[]) {
    initWindow(argc, argv);

    auto glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
        std::cerr << "ERROR: " << glewGetErrorString(glewInitResult) << '\n';
        std::exit(EXIT_FAILURE);
    }

    std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << '\n';

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void initWindow(int argc, char* argv[]) {
    glutInit(&argc, argv);

    glutInitContextVersion(4, 0);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    glutInitWindowSize(currentWidth, currentHeight);

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    windowHandle = glutCreateWindow(windowTitlePrefix);

    if (!windowHandle) {
        std::cerr << "ERROR: Could not create a new rendering window.\n";
        std::exit(EXIT_FAILURE);
    }

    glutReshapeFunc(resize);
    glutDisplayFunc(render);
}

void resize(int width, int height) {
    currentWidth = width;
    currentHeight = height;
    glViewport(0, 0, currentWidth, currentHeight);
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glutSwapBuffers();
    glutPostRedisplay();
}
