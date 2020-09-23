#include "utils.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <system_error>

constexpr int GL_VERSION_MAJOR = 4;
constexpr int GL_VERSION_MINOR = 3;

namespace detail
{
std::string_view basename(std::string_view s)
{
    const auto pos = s.find_last_of("/\\");
    if (pos == std::string_view::npos)
        return s;
    else
        return s.substr(pos + 1);
}
}

namespace
{
void glfwErrorCallback(int err, const char *msg)
{
    std::cerr << "[" << err << "] " << msg << "\n";
    std::exit(EXIT_FAILURE);
}
}

GLFWwindow *initGLFW(int width, int height, std::string_view title)
{
    if (!glfwInit()) {
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
    auto window = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwMakeContextCurrent(window);
    if (auto err = glewInit(); err != GLEW_OK) {
        std::cerr << "glewInit failed\n";
        glfwDestroyWindow(window);
        return nullptr;
    }

    return window;
}

std::string readFile(std::string_view filename)
{
    std::ifstream f(filename);
    if (!f.is_open())
        throw std::system_error(
            0,
            std::iostream_category(),
            "Could not open file " + std::string(filename)
        );


    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

void reportOpenglErrorAndQuit(std::string_view call, GLenum err, std::string_view file, size_t line)
{
    std::cerr << call << " : [" << err << "] " << detail::basename(file) << ':' << line << '\n';
    exit(EXIT_FAILURE);
}

