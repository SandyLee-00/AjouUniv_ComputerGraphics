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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma comment(lib, "glew32s")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glfw3")
using namespace glm;
void render(GLFWwindow* window);
void init();
void mouseButtonCallback(GLFWwindow*, int, int, int);
void cursorMotionCallback(GLFWwindow*, double,double);
GLuint triangleVBO=0;
GLuint vertexArrayID=0;
GLuint indexVBOID=0;
GLuint normalVBO=0;
GLuint texCoordVBO=0;
GLuint diffTexID=0;
GLuint bumpTexID = 0;
Program program;
double lastX=0  , lastY=0 ;
float cameraDistance = 8;
glm::vec3 sceneCenter = glm::vec3(0, 0, 0);
float cameraYaw = 0.f;
float cameraPitch = 0.f;
float cameraFov = 30.f;

vec3 lightPos = vec3(3, 3, 3);
vec3 lightColor = vec3(1);
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
    loadJ3A("dwarf.j3a");
    //std::vector<glm::vec3> triVertData = { { 0, 1, 0 }, { -0.7,-0.7,0 }, { 0.7,-0.7,0 } };
    int texWidth, texChannels,texHeight;
    void* buffer = stbi_load(diffuseMap[0].c_str(), &texWidth, &texHeight, &texChannels, 4);
    

    glGenTextures(1, &diffTexID);
    glBindTexture(GL_TEXTURE_2D, diffTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(buffer);

    buffer = stbi_load(bumpMap[0].c_str(), &texWidth, &texHeight, &texChannels, 4);


    glGenTextures(1, &bumpTexID);
    glBindTexture(GL_TEXTURE_2D, bumpTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(buffer);
    
    program.loadShaders("shader.vert", "shader.frag");
    //  triVertData.push_back(vec3(-0.7, -0.7, 0));
     // triVertData.push_back(vec3(0.7, -0.7, 0));
     // triVertData.push_back(vec3(0, 1, 0));

    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);


    glGenBuffers(1, &triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(vec3), vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(vec3), normals[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    
    glGenBuffers(1, &texCoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(vec2), texCoords[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &indexVBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0] * sizeof(u32vec3), triangles[0], GL_STATIC_DRAW);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_FRAMEBUFFER_SRGB);
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
    //GLuint loc = glGetUniformLocation(program.programID, "modelMat");
    
    mat4 projMat = perspective(cameraFov * 3.1415926535f / 180, w / float(h), 0.01f, 100.f);
    vec3 cameraPostition = vec3(0, 0, cameraDistance);
    cameraPostition = vec3(rotate(cameraPitch, vec3(-1, 0, 0)) * vec4(cameraPostition, 1));
    cameraPostition = vec3(rotate(cameraYaw, vec3(0, 1, 0)) * vec4(cameraPostition, 1));
    mat4 viewMat = lookAt(cameraPostition, sceneCenter, vec3(0, 1, 0));

    rotAngle += 0.1 / 180.f * 3.141592;
    GLuint loc = glGetUniformLocation(program.programID, "modelMat");

    //glUniformMatrix4fv(loc, 1, 0, value_ptr(rotate(90 / 180.f * 3.141592f, vec3(1, 0, 0))));

    loc = glGetUniformLocation(program.programID, "viewMat");
    glUniformMatrix4fv(loc, 1, 0, value_ptr(viewMat));

    loc = glGetUniformLocation(program.programID, "projMat");
    glUniformMatrix4fv(loc, 1, 0, value_ptr(projMat));
    loc = glGetUniformLocation(program.programID, "cameraPos");
    glUniform3fv(loc, 1, value_ptr(cameraPostition));
    loc = glGetUniformLocation(program.programID, "lightPos");
    glUniform3fv(loc, 1, value_ptr(lightPos));
    loc = glGetUniformLocation(program.programID, "lightColor");
    glUniform3fv(loc, 1, value_ptr(lightColor));

    glActiveTexture(GL_TEXTURE0+2);
    glBindTexture(GL_TEXTURE_2D, diffTexID);
    loc = glGetUniformLocation(program.programID, "diffTex");
    glUniform1i(loc, 2);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, bumpTexID);
    loc = glGetUniformLocation(program.programID, "bumpTex");
    glUniform1i(loc, 1);

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