#include <chrono>
#include "utils.h"

constexpr auto numVAOs = 1;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint createShaderProgram() {
    GLuint vShader = GL_CK(glCreateShader(GL_VERTEX_SHADER));
    GLuint fShader = GL_CK(glCreateShader(GL_FRAGMENT_SHADER));
    const std::string vShaderSource(readFile("ch2-vert.glsl"));
    const std::string fShaderSource(readFile("ch2-frag.glsl"));
    auto vShaderStr = vShaderSource.c_str();
    auto fShaderStr = fShaderSource.c_str();

    GL_CK_V(glShaderSource(vShader, 1, &vShaderStr, nullptr));
    GL_CK_V(glShaderSource(fShader, 1, &fShaderStr, nullptr));
    GL_CK_V(glCompileShader(vShader));
    checkShaderCompile(vShader);
    GL_CK_V(glCompileShader(fShader));
    checkShaderCompile(fShader);

    auto program = GL_CK(glCreateProgram());
    GL_CK_V(glAttachShader(program, vShader));
    GL_CK_V(glAttachShader(program, fShader));
    GL_CK_V(glLinkProgram(program));
    checkProgram(program);
    return program;
}

void init(GLFWwindow *window) {
    (void) window;
    renderingProgram = createShaderProgram();
    GL_CK_V(glGenVertexArrays(numVAOs, vao));
    GL_CK_V(glBindVertexArray(vao[0]));
}

template<typename Clock, typename Duration> void
display(GLFWwindow *window, std::chrono::time_point<Clock, Duration> currentTime) {
    GL_CK_V(glUseProgram(renderingProgram));
    GL_CK_V(glDrawArrays(GL_TRIANGLES, 0, 3));
}

int main(void) {
    auto window = initGl(600, 600, "Chapter 2 - program 1");
    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, std::chrono::steady_clock::now());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
