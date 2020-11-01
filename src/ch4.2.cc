#include "utils.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <iostream>
#include <cmath>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace {
constexpr auto numVAOs = 1;
constexpr auto numVBOs = 1;

glm::vec3 camera;
glm::vec3 cubeLoc;
glm::vec4 U, V, N;
GLuint renderingProgram;
std::array<GLuint, numVAOs> vao;
std::array<GLuint, numVBOs> vbo;

constexpr std::array cubeVertexPositions{
    -1.0F,  1.0F, -1.0F, -1.0F, -1.0F, -1.0F,  1.0F, -1.0F, -1.0F,
     1.0F, -1.0F, -1.0F,  1.0F,  1.0F, -1.0F, -1.0F,  1.0F, -1.0F,

     1.0F, -1.0F, -1.0F,  1.0F, -1.0F,  1.0F,  1.0F,  1.0F, -1.0F,
     1.0F, -1.0F,  1.0F,  1.0F,  1.0F,  1.0F,  1.0F,  1.0F, -1.0F,

     1.0F, -1.0F,  1.0F, -1.0F, -1.0F,  1.0F,  1.0F,  1.0F,  1.0F,
    -1.0F, -1.0F,  1.0F, -1.0F,  1.0F,  1.0F,  1.0F,  1.0F,  1.0F,

    -1.0F, -1.0F,  1.0F, -1.0F, -1.0F, -1.0F, -1.0F,  1.0F,  1.0F,
    -1.0F, -1.0F, -1.0F, -1.0F,  1.0F, -1.0F, -1.0F,  1.0F,  1.0F,

    -1.0F, -1.0F,  1.0F,  1.0F, -1.0F,  1.0F,  1.0F, -1.0F, -1.0F,
     1.0F, -1.0F, -1.0F, -1.0F, -1.0F, -1.0F, -1.0F, -1.0F,  1.0F,

    -1.0F,  1.0F, -1.0F,  1.0F,  1.0F, -1.0F,  1.0F,  1.0F,  1.0F,
     1.0F,  1.0F,  1.0F, -1.0F,  1.0F,  1.0F, -1.0F,  1.0F, -1.0F,
};

constexpr std::array parallelogramVertexPositions{
    -0.75F,  1.0F, -0.75F, -1.0F, -1.0F, -1.0F,  1.0F, -1.0F, -1.0F,
     1.0F, -1.0F, -1.0F,  0.75F,  1.0F, -0.75F, -0.75F,  1.0F, -0.75F,

     1.0F, -1.0F, -1.0F,  1.0F, -1.0F,  1.0F,  0.75F,  1.0F, -0.75F,
     1.0F, -1.0F,  1.0F,  0.75F,  1.0F,  0.75F,  0.75F,  1.0F, -0.75F,

     1.0F, -1.0F,  1.0F, -1.0F, -1.0F,  1.0F,  0.75F,  1.0F,  0.75F,
    -1.0F, -1.0F,  1.0F, -0.75F,  1.0F,  0.75F,  0.75F,  1.0F,  0.75F,

    -1.0F, -1.0F,  1.0F, -1.0F, -1.0F, -1.0F, -0.75F,  1.0F,  0.75F,
    -1.0F, -1.0F, -1.0F, -0.75F,  1.0F, -0.75F, -0.75F,  1.0F,  0.75F,

    -1.0F, -1.0F,  1.0F,  1.0F, -1.0F,  1.0F,  1.0F, -1.0F, -1.0F,
     1.0F, -1.0F, -1.0F, -1.0F, -1.0F, -1.0F, -1.0F, -1.0F,  1.0F,

    -0.75F,  1.0F, -0.75F,  0.75F,  1.0F, -0.75F,  0.75F,  1.0F,  0.75F,
     0.75F,  1.0F,  0.75F, -0.75F,  1.0F,  0.75F, -0.75F,  1.0F, -0.75F,
};

constexpr auto vertexPositions = cubeVertexPositions;
}

void setupVertices() {

    GL_CK_V(glGenVertexArrays(numVAOs, vao.data()));
    GL_CK_V(glBindVertexArray(vao[0]));
    GL_CK_V(glGenBuffers(numVBOs, vbo.data()));

    GL_CK_V(glBindBuffer(GL_ARRAY_BUFFER, vbo[0]));
    GL_CK_V(glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(vertexPositions[0]), vertexPositions.data(), GL_STATIC_DRAW));
}

void init() {
    renderingProgram = createProgram("ch4.2-vert.glsl", "ch4.1-frag.glsl");
    camera = {0.0f, 0.0f, 24.0f};
    cubeLoc = {0.0f, -2.0f, 0.0f};
    U = {1.0f, 0.0f, 0.0f, 0.0f};
    V = {0.0f, 1.0f, 0.0f, 0.0f};
    N = {0.0f, 0.0f, 1.0f, 0.0f};
    setupVertices();
}

void display(GLFWwindow *window) {
    constexpr auto numObjects = 24;
    const auto currentTime = static_cast<float>(glfwGetTime());

    GL_CK_V(glClear(GL_DEPTH_BUFFER_BIT));
    GL_CK_V(glClear(GL_COLOR_BUFFER_BIT));
    GL_CK_V(glUseProgram(renderingProgram));

    const auto mLoc = GL_CK(glGetUniformLocation(renderingProgram, "m_matrix"));
    const auto vLoc = GL_CK(glGetUniformLocation(renderingProgram, "v_matrix"));
    const auto projLoc = GL_CK(glGetUniformLocation(renderingProgram, "proj_matrix"));
    const auto tfLoc = GL_CK(glGetUniformLocation(renderingProgram, "tf"));

    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    const auto aspect = static_cast<float>(width) / static_cast<float>(height);
    auto pMat = glm::perspective(1.0472F /* 60 degrees */, aspect, 0.1F, 1000.0F);

    auto vMat = glm::translate(glm::mat4(1.0F), -camera);
    //auto vMat = glm::mat4{U, V, N, {0.0f, 0.0f, 0.0f, 1.0f}} * glm::translate(glm::mat4(1.0f), -camera);

    //auto mMat = glm::translate(glm::mat4(1.0f), cubeLoc);
    auto mMat = glm::mat4(1.0f);

    GL_CK_V(glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(mMat)));
    GL_CK_V(glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat)));
    GL_CK_V(glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat)));
    GL_CK_V(glUniform1f(tfLoc, currentTime));

    GL_CK_V(glBindBuffer(GL_ARRAY_BUFFER, vbo[0]));
    GL_CK_V(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr));
    GL_CK_V(glEnableVertexAttribArray(0));

    GL_CK_V(glEnable(GL_DEPTH_TEST));
    GL_CK_V(glDepthFunc(GL_LEQUAL));
    GL_CK_V(glDrawArraysInstanced(GL_TRIANGLES, 0, vertexPositions.size()/3, numObjects));
}

int main() {
    constexpr auto WIDTH = 600;
    constexpr auto HEIGHT = 600;
    auto window = initGl(WIDTH, HEIGHT, "Chapter 4 = program 1");
    init();

    while (0 == glfwWindowShouldClose(window)) {
        display(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
