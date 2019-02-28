//
//  main.cpp
//  openGL-T10-cam
//
//  Created by Lax Zhang on 2019/2/27.
//  Copyright © 2019 Lax Zhang. All rights reserved.
//
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "header/stb/stb.cpp"
#include "header/shader/shader.hpp"
#include "header/camera/camera.hpp"

using namespace std;

void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);
void mouseCallback(GLFWwindow*, double, double);
void scrollCallback(GLFWwindow *, double, double);

const int window_width = 800;
const int  window_height = 500;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = window_width / 2;
float lastY = window_height / 2;
bool firstMouse = true;

// timing
float deltaTime =0.0f;
float lastTime = 0.0f;

int main(int argc, const char * argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // 创建窗口
    GLFWwindow *window = glfwCreateWindow(window_width, window_height, "GLFW TEST", NULL, NULL);
    if (window == NULL) {
        cout<< "Failed Create Window" << endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    // 窗口调整大小 回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // 初始化GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "Failed init Glew." << endl;
        return -1;
    }
    
    // 编译着色器
    Shader shader("shaders/shader1.vs", "shaders/shader1.fs");
    
    // 正方体的36个顶点。 (一个面6个顶点)
    float vertices[] = {
        // 位置              // 纹理坐标
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    
    // 顶点数组对象
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // 1. 绑定VAO
    glBindVertexArray(VAO);
    // 2. 复制顶点数组到顶点缓冲中
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 3. 设置顶点属性
    
    // 位置变量设置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*(sizeof(float)), (void*)0);
    glEnableVertexAttribArray(0);
    
    
    shader.use();
    
    // 若不启用 GL_DEPTH_TEST 则会出现物体的后静和前景覆盖的问题
    glEnable(GL_DEPTH_TEST);
    
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    shader.setMat4("view", view);
    
    deltaTime = lastTime = 0.0f;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    while (!glfwWindowShouldClose(window)) {
        float current = glfwGetTime();
        deltaTime = current - lastTime;
        processInput(window); // 处理输入事件
        glfwSetCursorPosCallback(window, mouseCallback); // 设置鼠标回调函数
        glfwSetScrollCallback(window, scrollCallback); // 设置鼠标滚轮回调函数
        
        // 渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader.use();
        // model， view， projection矩阵的创建
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        
        projection = glm::perspective(glm::radians(camera.Zoom), (float) window_width / (float)window_height, 0.1f, 100.0f);
        
        view = camera.getViewMatrix();
        // 传入着色器
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        
        
        glBindVertexArray(VAO);
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        glfwSwapBuffers(window); // 颜色缓冲交换
        glfwPollEvents(); // 处理事件
        lastTime = current;
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);
}

void mouseCallback(GLFWwindow* window, double x_pos, double y_pos){
    if (firstMouse) {
        lastX = x_pos;
        lastY = y_pos;
        firstMouse = false;
    }
    
    float x_offset = x_pos - lastX;
    float y_offset = lastY - y_pos;
    lastX = x_pos;
    lastY = y_pos;
    
    camera.processMouseMovement(x_offset, y_offset);
}

void scrollCallback(GLFWwindow *window, double x_offset, double y_offset){
    camera.processMouseScroll(y_offset);
    
}
