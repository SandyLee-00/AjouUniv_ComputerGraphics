#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include "toys.h"
#include <vector>

using namespace std;
using namespace glm;

void render(GLFWwindow* win);
void init();

float clearB = .0f;
Program program;

GLuint vertBuf = 0;
GLuint vertArray = 0;
GLuint triBuf = 0;

GLuint cir_vertBuf = 0;
GLuint cir_vertArray = 0;
GLuint cir_Buf = 0;


int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* win = glfwCreateWindow(800, 600, "Test", nullptr, nullptr);
    glfwMakeContextCurrent(win);
    glewInit();
    glfwSwapInterval(1);
    init();

    while (!glfwWindowShouldClose(win)) {
        clearB += 0.05;
        if (clearB > 1.f) {
            clearB = 0.f;
        }
        render(win);
        glfwPollEvents();
    }
    glfwTerminate();
}

void init() {
    program.loadShaders("shader.vert", "shader.frag");

    /*std::vector<glm::vec3> vert;

    vert.push_back(glm::vec3(0.8, 0.7, 0));
    vert.push_back(glm::vec3(0.6, 0.4, 0));
    vert.push_back(glm::vec3(1.0, 0.4, 0));

    std::vector<glm::u32vec3> triangles;
    triangles.push_back(glm::u32vec3(0, 1, 2));

    glGenBuffers(1, &vertBuf);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
    glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(glm::vec3), vert.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &vertArray);
    glBindVertexArray(vertArray);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);

    glGenBuffers(1, &triBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(glm::u32vec3), triangles.data(), GL_STATIC_DRAW);*/

    double theta = 0;
    float r = 0.8;

    vector<vec3> circle_vert;
    circle_vert.push_back(vec3(0, 0, 0));

    for (int i = 0; i < 360; i++) {
        theta = i;
        circle_vert.push_back(r * vec3(cos(theta), sin(theta), 0));
    }

    vector<u32vec3> circles;

    for (int i = 1; i < 361; i++) {
        if (i == 360) {
            circles.push_back(u32vec3(0, i, 1));
        }
        circles.push_back(u32vec3(0, i, i + 1));
    }

    glGenBuffers(1, &cir_vertArray);
    glBindBuffer(GL_ARRAY_BUFFER, cir_vertArray);
    glBufferData(GL_ARRAY_BUFFER, circle_vert.size() * sizeof(vec3), circle_vert.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &cir_vertBuf);
    glBindVertexArray(cir_vertBuf);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, cir_vertArray);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);

    glGenBuffers(1, &cir_Buf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cir_Buf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, circles.size() * sizeof(u32vec3), circles.data(), GL_STATIC_DRAW);


}

void render(GLFWwindow* win) {
    int w, h;
    glfwGetFramebufferSize(win, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0, 0, 0.2, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program.programID);

    /*glBindVertexArray(vertArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triBuf);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);*/

    glBindVertexArray(cir_vertArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cir_Buf);
    glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(win);

}