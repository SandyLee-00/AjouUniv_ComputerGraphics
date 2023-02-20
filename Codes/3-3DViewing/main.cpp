#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "toys.h"
#include <vector>
#include "j3a.hpp"

using namespace std;
using namespace glm;
const float PI = 3.14159265388979;
float blue = 0;

void render(GLFWwindow* window);
void init();

Program program;

GLuint vertBuf = 0;
GLuint vertArray = 0;
GLuint triBuf = 0;

float theta = 0;
float phi = 0;
float fovy = 60;
float cameraDistance = 1;

vec3 cameraPosition = vec3(0, 0, 5);

double oldX;
double oldY;

void cursorCallback(GLFWwindow* window, double xpos, double ypos) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)) {
        printf("%f %f\n", xpos, ypos);
        if (theta < -90 && theta > 90) {
            theta -= (xpos - oldX) / width * PI;
        }
        if (phi < -90 && phi > 90) {
            phi -= (ypos - oldY) / height * PI;
        }

    }
    oldX = xpos;
    oldY = ypos;

}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (fovy < 170 && fovy > 10) {
        fovy = fovy * pow(1.01, -yoffset);
    }
    //cameraDistance = cameraDistance * pow(1.01, -yoffset);
}

int main() {
    

    if (!glfwInit()) exit(EXIT_FAILURE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 800, "Assignment2", NULL, NULL);
    glfwSetCursorPosCallback(window, cursorCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwMakeContextCurrent(window);
    glewInit();
    init();

    while (!glfwWindowShouldClose(window)) {
        render(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

void init() {
    loadJ3A("dragon.j3a");
    program.loadShaders("shader.vert", "shader.frag");

    glGenBuffers(1, &vertBuf);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(glm::vec3), vertices[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &vertArray);
    glBindVertexArray(vertArray);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);

    glGenBuffers(1, &triBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0] * sizeof(glm::u32vec3), triangles[0], GL_STATIC_DRAW);

    
}

void render(GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    vec3 cameraPos = vec3(
        rotate(theta, vec3(0, 1, 0))
        * rotate(phi, vec3(1, 0, 0))
        *vec4(cameraPosition*cameraDistance, 1));

    mat4 viewMat = lookAt(cameraPos, vec3(0, 0, 0), vec3(0, 1, 0));
    mat4 projMat = perspective(fovy * PI / 180, width / float(height), 0.01f, 100.f);
    mat4 modelMat = mat4(1);


    glViewport(0, 0, width, height);
    glClearColor(0, 0, 0.2, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program.programID);
    GLuint loc;
    loc = glGetUniformLocation(program.programID, "modelMat");
    glUniformMatrix4fv(loc, 1, false, glm::value_ptr(modelMat));
    loc = glGetUniformLocation(program.programID, "viewMat");
    glUniformMatrix4fv(loc, 1, false, glm::value_ptr(viewMat));
    loc = glGetUniformLocation(program.programID, "projMat");
    glUniformMatrix4fv(loc, 1, false, glm::value_ptr(projMat));

    glBindVertexArray(vertArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triBuf);
    glDrawElements(GL_TRIANGLES, nTriangles[0]*3, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);

}