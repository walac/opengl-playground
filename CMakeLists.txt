cmake_minimum_required(VERSION 3.18.0)

project(OpenglPlayground CXX)
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)
include(cmake/StaticAnalyzers.cmake)
include(cmake/Sanitizers.cmake)

include(CheckCXXCompilerFlag)

function(check_compile_option option var)
    check_cxx_compiler_flag(${option} ${var})
    if (${var})
        add_compile_options(${option})
    endif()
endfunction()

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

check_compile_option(-Wno-deprecated HAS_NO_DEPRECATED)

# After digging into the glew-targets.cmake file for macOS I discover
# it is the cause for the error
# No rule to make target `/Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/OpenGL.framework'
# So we use pkg-config in mac
if (APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLEW REQUIRED IMPORTED_TARGET glew)
    include_directories(${GLEW_INCLUDE_DIRS})
    set(GLEW_LIBS PkgConfig::GLEW)
else()
    find_package(GLEW REQUIRED)
    set(GLEW_LIBS GLEW::GLEW)
endif()

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)

find_path(GLFW_INCLUDE_DIR GLFW)
include_directories(${GLFW_INCLUDE_DIR} ${GLM_INCLUDE_DIR})

set(LIBS
    glfw
    ${GLEW_LIBS}
    OpenGL::GL
)

add_executable(ch2 src/utils.cc src/ch2.cc)
target_link_libraries(ch2 PRIVATE ${LIBS})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/ch2-vert.glsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/ch2-frag.glsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(ch4.1 src/utils.cc src/ch4.1.cc)
target_link_libraries(ch4.1 PRIVATE ${LIBS})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/ch4.1-vert.glsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/ch4.1-frag.glsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(ch4.2 src/utils.cc src/ch4.2.cc)
target_link_libraries(ch4.2 PRIVATE ${LIBS})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/ch4.2-vert.glsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
