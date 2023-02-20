#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
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

float triangleSize = 0.3;
float rotspeed = 0;


int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* win = glfwCreateWindow(800, 800, "Test", nullptr, nullptr);
    glfwMakeContextCurrent(win);
    glewInit();
    glfwSwapInterval(1);
    init();

    while (!glfwWindowShouldClose(win)) {
        /*triangleSize += 0.05;
        if (triangleSize > 1) triangleSize = 0.3;*/

        rotspeed += 5 / 180.0 * 3.141592;

        render(win);
        glfwPollEvents();
    }
    glfwTerminate();
}

void init() {
    program.loadShaders("shader.vert", "shader.frag");

    std::vector<glm::vec3> vert;

    vert.push_back(glm::vec3(0.0, 0.7, 0));
    vert.push_back(glm::vec3(-0.7, -0.7, 0));
    vert.push_back(glm::vec3(0.7, -0.7, 0));

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(glm::u32vec3), triangles.data(), GL_STATIC_DRAW);

}

void render(GLFWwindow* win) {
    GLuint loc;
    GLuint rot;

    int w, h;
    glfwGetFramebufferSize(win, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0, 0, 0.2, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program.programID);

    /*printf("%f\n", triangleSize);*/
    printf("%f\n", rotspeed);

    /*loc = glGetUniformLocation(program.programID, "transf");
    mat3 transf = mat3(triangleSize, 0, 0, 0, triangleSize, 0, 0, 0, 1);
    glUniformMatrix3fv(loc, 1, false, value_ptr(transf));*/
    
    rot = glGetUniformLocation(program.programID, "transf");
    mat3 transf = mat3(
        cos(rotspeed), -sin(rotspeed), 0, 
        sin(rotspeed), cos(rotspeed), 0,
        0, 0, 1);

    glUniformMatrix3fv(rot, 1, false, value_ptr(transf));

    glBindVertexArray(vertArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triBuf);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(win);

}