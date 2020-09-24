#pragma once

#include <string>
#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>
#include <string_view>
#include <algorithm>
#include <type_traits>

namespace detail
{
inline std::string_view basename(std::string_view s) noexcept {
    const auto pos = s.find_last_of("/\\");
    if (pos == std::string_view::npos)
        return s;
    else
        return s.substr(pos + 1);
}
}

GLFWwindow *initGl(int width, int height, std::string_view title);
std::string readFile(std::string_view filename);
void reportOpenglErrorAndQuit(std::string_view call, GLenum err, std::string_view file, size_t line);
void checkShaderCompile(GLuint shader);
void checkProgram(GLuint program);

inline void
checkOpengl(std::string_view call, std::string_view file, size_t line) {
    if (auto err = glGetError(); err != 0)
        reportOpenglErrorAndQuit(call, err, file, line);
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

#ifdef __APPLE__
#ifdef glGenVertexArrays
#undef glGenVertexArrays
#endif
#define glGenVertexArrays glGenVertexArraysAPPLE

#ifdef glBindVertexArray
#undef glBindVertexArray
#endif
#define glBindVertexArray glBindVertexArrayAPPLE
#endif
