#define GLEW_STATIC
#define GLM_EXPERIMENTAL
#include <vector>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include "toys.h"
#include "j3a.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#pragma comment(lib, "glew32s")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glfw3")
void render(GLFWwindow* window);
void init();
void mouseButtonCallback(GLFWwindow*, int, int, int);
void cursorMotionCallback(GLFWwindow*, double,double);
GLuint triangleVBO;
GLuint vertexArrayID;
GLuint indexVBOID;
Program program;
int lastX=0, lastY = 0;
float cameraDistance = 3;
glm::vec3 sceneCenter = glm::vec3(0, 0, 0);
float cameraYaw = 0.f;
float cameraPitch = 0.f;
float cameraFov = 60.f;
int main(int argu, const char* argv[]) {
    
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "Assignment2", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorMotionCallback);
    glewInit();
    init();
    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window)) {
        render(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

}



using namespace glm;
using namespace std;
void init() {
    loadJ3A("bunny.j3a");
    //std::vector<glm::vec3> triVertData = { { 0, 1, 0 }, { -0.7,-0.7,0 }, { 0.7,-0.7,0 } };
    program.loadShaders("shader.vert", "shader.frag");
    //  triVertData.push_back(vec3(-0.7, -0.7, 0));
     // triVertData.push_back(vec3(0.7, -0.7, 0));
     // triVertData.push_back(vec3(0, 1, 0));

    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
    
    glGenBuffers(1, &triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(glm::vec3), vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


    glGenBuffers(1, &indexVBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0] * sizeof(glm::u32vec3), triangles[0], GL_STATIC_DRAW);



}


float rotAngle = 0;
void render(GLFWwindow* window) {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0, 0, .5, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program.programID);
    glEnable(GL_DEPTH_TEST);

    mat4 projMat = perspective(cameraFov * 3.141592f / 180,w/float(h),0.01f,100.f );
    vec3 cameraPostition = vec3(0, 0, cameraDistance);
    cameraPostition = vec3(rotate(cameraPitch, vec3(-1, 0, 0))*vec4(cameraPostition,1));
    cameraPostition = vec3(rotate(cameraYaw, vec3(0, 1, 0)) * vec4(cameraPostition, 1));
    mat4 viewMat = lookAt(cameraPostition, sceneCenter, vec3(0, 1, 0));
    
    rotAngle += 0.1 / 180.f*3.141592;
    GLuint loc = glGetUniformLocation(program.programID, "modelMat");
  //  mat4 rotMat = rotate(rotAngle, glm::vec3(0, 1, 0));
    glUniformMatrix4fv(loc, 1, 0, value_ptr(rotate(90/180.f*3.141592f,vec3(1,0,0))));

    loc = glGetUniformLocation(program.programID, "viewMat");
    glUniformMatrix4fv(loc, 1, 0, value_ptr(viewMat));

    loc = glGetUniformLocation(program.programID, "projMat");
    glUniformMatrix4fv(loc, 1, 0, value_ptr(projMat));

    //glUseProgram(program.programID);
    //glUniformMatrix4fv(loc, 1, 0, value_ptr(rotMat));
    
    glBindVertexArray(vertexArrayID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
    glDrawElements(GL_TRIANGLES, nTriangles[0] * 3, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);

}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
{
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        lastX = int(xpos);
        lastY=int(ypos);
        //printf("%d %d\n", lastX, lastY);
    }
}

void cursorMotionCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1 == GLFW_PRESS))
    {
        if(glfwGetKey(window,GLFW_KEY_LEFT_CONTROL)==GLFW_PRESS)
        {
            cameraDistance *= pow(1.001, ypos - lastY);
        }
        else {
            cameraPitch += (ypos - lastY) / 300;
                cameraPitch = glm::clamp(cameraPitch, -1.f, 1.f);
                cameraYaw -= (xpos - lastX) / 300;
               
        }
        lastX = int(xpos);
            lastY = int(ypos);
    }
}