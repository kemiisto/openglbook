#include <cstdlib>
#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GL/freeglut.h>

struct Vertex {
    float xyzw[4];
    float rgba[4];
};

const char* windowTitlePrefix = "Chapter 2";

int currentWidth = 800;
int currentHeight = 600;
int windowHandle = 0;
int frameCount = 0;

GLuint vertexShaderId;
GLuint fragmentShaderId;
GLuint programId;
GLuint vaoId;
GLuint vboId;

const GLchar* vertexShader = {
    "#version 400\n"
    "\n"
    "layout(location=0) in vec4 in_Position;\n"
    "layout(location=1) in vec4 in_Color;\n"
    "out vec4 ex_Color;\n"
    "\n"
    "void main(void) {\n"
    "  gl_Position = in_Position;\n"
    "  ex_Color = in_Color;\n"
    "}\n"
};

const GLchar* fragmentShader = {
    "#version 400\n"
    "\n"
    "in vec4 ex_Color;\n"
    "out vec4 out_Color;\n"
    "\n"
    "void main() {\n"
    "  out_Color = ex_Color;\n"
    "}\n"
};

void initialize(int, char*[]);
void initWindow(int, char*[]);
void resize(int, int);
void render();
void timer(int);
void idle();
void cleanup();
void createVBO();
void destroyVBO();
void createShaders();
void destroyShaders();

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

    createShaders();
    createVBO();

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
    glutCloseFunc(cleanup);
}

void resize(int width, int height) {
    currentWidth = width;
    currentHeight = height;
    glViewport(0, 0, currentWidth, currentHeight);
}

void render() {
    frameCount++;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

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

void cleanup() {
    destroyShaders();
    destroyVBO();
}

void createVBO() {
    Vertex vertices[] = {
        { { -0.8f, -0.8f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { {  0.0f,  0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { {  0.8f, -0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
    };

    const auto vertexSize = sizeof(vertices[0]);
    const auto rgbOffset = sizeof(vertices[0].xyzw);

    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vertexSize, nullptr);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (void*)rgbOffset);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    auto errorCheckValue = glGetError();
    if (errorCheckValue != GL_NO_ERROR) {
        std::cerr << "ERROR: Could not create a VBO: " << gluErrorString(errorCheckValue) << '\n';
        std::exit(EXIT_FAILURE);
    }
}

void destroyVBO() {
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vboId);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vaoId);

    auto errorCheckValue = glGetError();
    if (errorCheckValue != GL_NO_ERROR) {
        std::cerr << "ERROR: Could not destroy the VBO: " << gluErrorString(errorCheckValue) << '\n';
        std::exit(EXIT_FAILURE);
    }
}

void createShaders() {
    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexShader, nullptr);
    glCompileShader(vertexShaderId);

    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragmentShader, nullptr);
    glCompileShader(fragmentShaderId);

    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);
    glUseProgram(programId);

    auto errorCheckValue = glGetError();
    if (errorCheckValue != GL_NO_ERROR) {
        std::cerr << "ERROR: Could not create the shaders: " << gluErrorString(errorCheckValue) << '\n';
        std::exit(EXIT_FAILURE);
    }
}

void destroyShaders() {
    glUseProgram(0);

    glDetachShader(programId, vertexShaderId);
    glDetachShader(programId, fragmentShaderId);

    glDeleteShader(fragmentShaderId);
    glDeleteShader(vertexShaderId);

    glDeleteProgram(programId);

    auto errorCheckValue = glGetError();
    if (errorCheckValue != GL_NO_ERROR) {
        std::cerr << "ERROR: Could not destroy the shaders: " << gluErrorString(errorCheckValue) << '\n';
        std::exit(EXIT_FAILURE);
    }
}
