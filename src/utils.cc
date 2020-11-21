#include <atomic>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <system_error>
#include <chrono>
#include <vector>
#include <SOIL2/SOIL2.h>
#include "utils.h"

constexpr int GL_VERSION_MAJOR = 4;
constexpr int GL_VERSION_MINOR =
#ifdef __APPLE__
1
#else
3
#endif
;

namespace
{
void glfwErrorCallback(int err, const char *msg) {
    std::cerr << "[" << err << "] " << msg << "\n";
    std::exit(EXIT_FAILURE);
}
}

GLFWwindow *initGl(int width, int height, std::string_view title) {
    if (glfwInit() == 0) {
        std::cerr << "glfwInit failed\n";
        return nullptr;
    }

    glfwSetErrorCallback(glfwErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    auto *window = glfwCreateWindow(width, height, title.data(), NULL, NULL);

#ifdef __APPLE__
    int actualScreenWidth = 0, actualScreenHeight = 0;
    glfwGetFramebufferSize(window, &actualScreenWidth, &actualScreenHeight);
#endif

    glfwMakeContextCurrent(window);

#ifdef __APPLE__
    glViewport(0,0, actualScreenWidth, actualScreenHeight);
    glewExperimental = GL_TRUE;
#endif

    if (auto err = glewInit(); err != GLEW_OK) {
        std::cerr << "glewInit failed\n";
        glfwDestroyWindow(window);
        return nullptr;
    }

    glfwSwapInterval(0);

    return window;
}

std::string readFile(std::string_view filename) {
    std::ifstream f(filename.data());
    if (!f.is_open()) {
        throw std::system_error(
            0,
            std::iostream_category(),
            "Could not open file " + std::string(filename)
        );
    }


    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

GLuint loadImage(std::string_view filename) {
    const auto textureId = SOIL_load_OGL_texture(filename.data(),
        SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (textureId == 0) {
        throw std::system_error(
            0,
            std::iostream_category(),
            "Could not open file " + std::string(filename)
        );
    }
}

void reportOpenglErrorAndQuit(std::string_view call, GLenum err, std::string_view file, size_t line) {
    std::cerr << call << " : [" << err << "] " << detail::basename(file) << ':' << line << '\n';
    exit(EXIT_FAILURE);
}

void checkShaderCompile(GLuint shader) {
    auto len = 0;
    GL_CK_V(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len));
    if (len > 0) {
        std::vector<char> buffer(len);
        GL_CK_V(glGetShaderInfoLog(shader, len, nullptr, &buffer[0]));
        std::cerr << &buffer[0] << '\n';
    }
}

void checkProgram(GLuint program) {
    auto len = 0;
    GL_CK_V(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len));
    if (len > 0) {
        std::vector<char> buffer(len);
        GL_CK_V(glGetProgramInfoLog(program, len, nullptr, &buffer[0]));
        std::cerr << &buffer[0] << '\n';
    }
}

GLuint createProgram(std::string_view vertexSourceFile, std::string_view fragmentSourceFile) {
    const auto vertexSource = readFile(vertexSourceFile);
    const auto fragmentSource = readFile(fragmentSourceFile);
    const auto *vShaderStr = vertexSource.c_str();
    const auto *fShaderStr = fragmentSource.c_str();
    auto vShader = GL_CK(glCreateShader(GL_VERTEX_SHADER));
    auto fShader = GL_CK(glCreateShader(GL_FRAGMENT_SHADER));
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

FPSCounter::FPSCounter()
    : counter(0)
    , fut(promise.get_future())
    , thread(&FPSCounter::fpsPrinter, this)
{
}

void FPSCounter::count() {
    counter.fetch_add(1, std::memory_order_relaxed);
}

FPSCounter::~FPSCounter() {
    promise.set_value();
    thread.join();
}

void FPSCounter::fpsPrinter() {
    using namespace std::chrono_literals;
    while (fut.wait_for(1s) == std::future_status::timeout) {
        auto count = counter.exchange(0, std::memory_order_acquire);
        std::cout << count << " fps\n";
    }
}
