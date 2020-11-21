#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#if __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/gl.h>
#endif
#include <string_view>
#include <algorithm>
#include <type_traits>
#include <atomic>
#include <thread>
#include <future>

namespace detail
{
inline std::string_view basename(std::string_view s) noexcept {
    const auto pos = s.find_last_of("/\\");
    if (pos == std::string_view::npos) {
        return s;
    }
    return s.substr(pos + 1);
}
}

GLFWwindow *initGl(int width, int height, std::string_view title);
std::string readFile(std::string_view filename);
void reportOpenglErrorAndQuit(std::string_view call, GLenum err, std::string_view file, size_t line);
void checkShaderCompile(GLuint shader);
void checkProgram(GLuint program);
GLuint createProgram(std::string_view vertexSourceFile, std::string_view fragmentSourceFile);
GLuint loadImage(std::string_view filename);

inline void
checkOpengl(std::string_view call, std::string_view file, size_t line) {
    if (auto err = glGetError(); err != 0) {
        reportOpenglErrorAndQuit(call, err, file, line);
    }
}

template<typename T>
T checkOpengl(T retVal, std::string_view call, std::string_view file, size_t line) {
    checkOpengl(call, file, line);
    return retVal;
}

#define GL_CK(call) checkOpengl(call, #call, __FILE__, __LINE__)
#define GL_CK_V(call) \
    do { \
        call; \
        checkOpengl(#call, __FILE__, __LINE__); \
    } while (0)

class FPSCounter final {
public:
    FPSCounter();
    ~FPSCounter();
    void count();

private:
    void fpsPrinter();

    std::atomic_size_t counter;
    std::promise<void> promise;
    std::future<void> fut;
    std::thread thread;
};
