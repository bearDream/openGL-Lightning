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

#include "header/texture/texture.hpp"
//#include "header/stb/stb.cpp"
#include "header/shader/shader.hpp"
#include "header/camera/camera.hpp"

using namespace std;

void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);
void mouseCallback(GLFWwindow*, double, double);
void scrollCallback(GLFWwindow *, double, double);

const int window_width = 800;
const int  window_height = 500;
char cube_texture[255] = "/Users/laxzhang/Code/openGL/openGL-Lightning/openGL-Lightning/container2.png";
char border_texture[255] = "/Users/laxzhang/Code/openGL/openGL-Lightning/openGL-Lightning/container2_specular.png";


// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = window_width / 2;
float lastY = window_height / 2;
bool firstMouse = true;

// timing
float deltaTime =0.0f;
float lastTime = 0.0f;

// 光源位置
glm::vec3 lightPos = glm::vec3(1.2, 1.0f, 2.0f);

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
    
    
    // 若不启用 GL_DEPTH_TEST 则会出现物体的后静和前景覆盖的问题
    glEnable(GL_DEPTH_TEST);
    
    // 编译着色器
    Shader shader("shaders/shader1.vs", "shaders/shader1.fs");
    Shader lampShader("shaders/shader_lighter.vs", "shaders/shader_lighter.fs");
    
    // 正方体的36个顶点。 (一个面6个顶点)
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 1.2f,  2.0f, -0.9f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };
    unsigned int lightPosLen = sizeof(pointLightPositions)/sizeof(pointLightPositions[0]);
    
    // 顶点数组对象
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*(sizeof(float)), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*(sizeof(float)), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*(sizeof(float)), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    unsigned int texture_cube = loadTexture(cube_texture);
    unsigned int texture_border = loadTexture(border_texture);
    
    // 光源着色器
    unsigned int lighterVAO;
    glGenVertexArrays(1, &lighterVAO);
    glBindVertexArray(lighterVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*(sizeof(float)), (void*)0);
    glEnableVertexAttribArray(0);
    
    deltaTime = lastTime = 0.0f;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    while (!glfwWindowShouldClose(window)) {
        float current = glfwGetTime();
        deltaTime = current - lastTime;
        processInput(window); // 处理输入事件
        glfwSetCursorPosCallback(window, mouseCallback); // 设置鼠标回调函数
        glfwSetScrollCallback(window, scrollCallback); // 设置鼠标滚轮回调函数
        
        // 渲染指令
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        shader.use();
        shader.setVec3("viewPos", camera.camPos);
        shader.setInt1("material.diffuse", 0);
        shader.setInt1("material.specular", 1.0f);
        shader.setFloat1("material.shininess", 64.0f);
        
        // 设置定向光源
        shader.setDirectionLight(lightPos, glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(1.0f));
        
        // 设置手电筒光源
        shader.setSpotLight(camera.camFront, camera.camPos, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)), glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(1.0f));
        
        // 设置点光源覆盖距离 默认50。其他距离的参数见
        // http://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
        shader.setPointLight(pointLightPositions, 1.0f, 0.09f, 0.032f, glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(1.0f) , 4);
        
        // model， view， projection矩阵的创建
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        
        projection = glm::perspective(glm::radians(camera.Zoom), (float) window_width / (float)window_height, 0.1f, 100.0f);
        
        view = camera.getViewMatrix();
        // 传入着色器
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        
        // 绘制物体
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_cube);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_border);
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++) {
            
            model = glm::translate(model, cubePositions[i]);
            float angel = 20.0f * i;
            model = glm::rotate(model, glm::radians(angel), glm::vec3(1.0f, 0.3f, 0.5f));
            
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        // 光源设置
        lampShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lampShader.setMat4("view", view);
        lampShader.setMat4("projection", projection);
        // 绘制光源
        glBindVertexArray(lighterVAO);
        for (int i = 0; i < lightPosLen; i++) {
            model = glm::translate(model, pointLightPositions[i]);
            lampShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        glfwSwapBuffers(window); // 颜色缓冲交换
        glfwPollEvents(); // 处理事件
        lastTime = current;
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lighterVAO);
    glDeleteBuffers(1, &VBO);
    
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
