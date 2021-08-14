#include <cstdlib>
#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GL/freeglut.h>

const char* windowTitlePrefix = "Chapter 1";

int currentWidth = 800;
int currentHeight = 600;
int windowHandle = 0;
int frameCount = 0;

void initialize(int, char*[]);
void initWindow(int, char*[]);
void resize(int, int);
void render();
void timer(int);
void idle();

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
    glutIdleFunc(idle);
    glutTimerFunc(0, timer, 0);
}

void resize(int width, int height) {
    currentWidth = width;
    currentHeight = height;
    glViewport(0, 0, currentWidth, currentHeight);
}

void render() {
    frameCount++;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glutSwapBuffers();
    glutPostRedisplay();
}

void timer(int value) {
    if (value) {
        std::ostringstream oss;
        oss << windowTitlePrefix << ": " << frameCount * 4 << " FPS @ " << currentWidth << " x " << currentHeight;
        glutSetWindowTitle(oss.str().c_str());
    }
    frameCount = 0;
    glutTimerFunc(250, timer, 1);
}

void idle() {
    glutPostRedisplay();
}
