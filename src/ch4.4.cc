#include "utils.h"
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <iostream>
#include <cmath>
#include <array>
#include <stack>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace {
glm::vec3 camera;
glm::vec3 cubeLoc, pyrLoc;
glm::vec4 U, V, N;
GLuint renderingProgram;
std::array<GLuint, 1> vao;
std::array<GLuint, 4> vbo;

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

constexpr std::array pyramidVertexPositions{
    -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f,

     1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,

    -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
};

constexpr std::array trapezoidVertexPositions{
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
}

void setupVertices() {

    GL_CK_V(glGenVertexArrays(vao.size(), vao.data()));
    GL_CK_V(glBindVertexArray(vao[0]));
    GL_CK_V(glGenBuffers(vbo.size(), vbo.data()));

    GL_CK_V(glBindBuffer(GL_ARRAY_BUFFER, vbo[1]));
    GL_CK_V(glBufferData(GL_ARRAY_BUFFER, cubeVertexPositions.size() * sizeof(cubeVertexPositions[0]), cubeVertexPositions.data(), GL_STATIC_DRAW));

    GL_CK_V(glBindBuffer(GL_ARRAY_BUFFER, vbo[0]));
    GL_CK_V(glBufferData(GL_ARRAY_BUFFER, pyramidVertexPositions.size() * sizeof(pyramidVertexPositions[0]), pyramidVertexPositions.data(), GL_STATIC_DRAW));

    GL_CK_V(glBindBuffer(GL_ARRAY_BUFFER, vbo[2]));
    GL_CK_V(glBufferData(GL_ARRAY_BUFFER, cubeVertexPositions.size() * sizeof(cubeVertexPositions[0]), cubeVertexPositions.data(), GL_STATIC_DRAW));

    GL_CK_V(glBindBuffer(GL_ARRAY_BUFFER, vbo[3]));
    GL_CK_V(glBufferData(GL_ARRAY_BUFFER, trapezoidVertexPositions.size() * sizeof(trapezoidVertexPositions[0]), trapezoidVertexPositions.data(), GL_STATIC_DRAW));
}

void init() {
    renderingProgram = createProgram("ch4.1-vert.glsl", "ch4.1-frag.glsl");
    camera = {0.0f, 0.0f, 24.0f};
    cubeLoc = {0.0f, -2.0f, 0.0f};
    pyrLoc = {2.0f, 2.0f, 0.0f};
    U = {1.0f, 0.0f, 0.0f, 0.0f};
    V = {0.0f, 1.0f, 0.0f, 0.0f};
    N = {0.0f, 0.0f, 1.0f, 0.0f};
    setupVertices();
}

void display(GLFWwindow *window) {
    std::stack<glm::mat4> stk;
    const auto currentTime = static_cast<float>(glfwGetTime());

    GL_CK_V(glClear(GL_DEPTH_BUFFER_BIT));
    GL_CK_V(glClear(GL_COLOR_BUFFER_BIT));
    GL_CK_V(glUseProgram(renderingProgram));

    auto mvLoc = GL_CK(glGetUniformLocation(renderingProgram, "mv_matrix"));
    auto projLoc = GL_CK(glGetUniformLocation(renderingProgram, "proj_matrix"));

    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    const auto aspect = static_cast<float>(width) / static_cast<float>(height);
    auto pMat = glm::perspective(1.0472F /* 60 degrees */, aspect, 0.1F, 1000.0F);
    GL_CK_V(glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat)));

    auto vMat = glm::translate(glm::mat4(1.0F), -camera);
    stk.push(vMat);

    // sun
    stk.push(stk.top());
    stk.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    stk.push(stk.top());
    stk.top() *= glm::rotate(glm::mat4(1.0f), currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
    GL_CK_V(glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(stk.top())));
    GL_CK_V(glBindBuffer(GL_ARRAY_BUFFER, vbo[0]));
    GL_CK_V(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr));
    GL_CK_V(glEnableVertexAttribArray(0));
    GL_CK_V(glEnable(GL_DEPTH_TEST));
    GL_CK_V(glDepthFunc(GL_LEQUAL));
    GL_CK_V(glDrawArrays(GL_TRIANGLES, 0, pyramidVertexPositions.size()/3));
    stk.pop();

    // Planet
    stk.push(stk.top());
    stk.top() *= glm::translate(
        glm::mat4(1.0f),
        glm::vec3(std::sin(currentTime) * 8.0f, 0.0f, cos(currentTime) * 8.0f)
    );
    stk.push(stk.top());
    stk.top() *= glm::rotate(glm::mat4(1.0f), 1.5f*currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
    GL_CK_V(glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(stk.top())));
    GL_CK_V(glBindBuffer(GL_ARRAY_BUFFER, vbo[1]));
    GL_CK_V(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr));
    GL_CK_V(glEnableVertexAttribArray(0));
    GL_CK_V(glDrawArrays(GL_TRIANGLES, 0, cubeVertexPositions.size()/3));
    stk.pop();

    // moon
    stk.push(stk.top());
    stk.top() *= glm::translate(
        glm::mat4(1.0f),
        glm::vec3(
            0.0f,
            std::sin(currentTime) * 2.0f,
            std::cos(currentTime) * 2.0f
        )
    );
    stk.top() *= glm::rotate(glm::mat4(1.0f), currentTime, glm::vec3(0.0f, 0.0f, 1.0f));
    stk.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));
    GL_CK_V(glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(stk.top())));
    GL_CK_V(glBindBuffer(GL_ARRAY_BUFFER, vbo[2]));
    GL_CK_V(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr));
    GL_CK_V(glEnableVertexAttribArray(0));
    GL_CK_V(glDrawArrays(GL_TRIANGLES, 0, cubeVertexPositions.size()/3));

    // Second planet
    stk.pop(); stk.pop(); stk.pop();
    stk.push(stk.top());
    stk.top() *= glm::translate(
        glm::mat4(1.0f),
        glm::vec3(std::sin(1.5f * currentTime) * 3.0f, 0.0f, std::cos(1.5f * currentTime) * 3.0f)
    );
    stk.top() *= glm::rotate(glm::mat4(1.0f), 2.0f * currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
    stk.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
    GL_CK_V(glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(stk.top())));
    GL_CK_V(glBindBuffer(GL_ARRAY_BUFFER, vbo[3]));
    GL_CK_V(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr));
    GL_CK_V(glEnableVertexAttribArray(0));
    GL_CK_V(glDrawArrays(GL_TRIANGLES, 0, trapezoidVertexPositions.size()/3));
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
