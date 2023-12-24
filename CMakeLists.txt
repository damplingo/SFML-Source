cmake_minimum_required(VERSION 3.24)

project(sfml-source LANGUAGES CXX)

set(SFML_STATIC_LIBRARIES TRUE)
option(BUILD_SHARED_LIBS ON)

find_package(OpenGL REQUIRED)
find_package(TinyXML REQUIRED)

add_subdirectory(${PROJECT_SOURCE_DIR}/SFML-2.6.0)
add_subdirectory(${PROJECT_SOURCE_DIR}/TinyXML)

add_executable(sfml-source main.cpp Jumpman.cpp Jumpman.h Animation.cpp Animation.h Level.cpp Level.h
        Enemy.cpp Enemy.h GameMenu.cpp GameMenu.h Moving_platform.cpp Moving_platform.h
        TinyXML/tinyxml.h TinyXML/tinyxml.cpp TinyXML/tinystr.cpp TinyXML/tinystr.h TinyXML/tinyxmlerror.cpp TinyXML/tinyxmlparser.cpp)

target_link_libraries(sfml-source PRIVATE sfml-audio sfml-graphics sfml-window sfml-system ${OPENGL_LIBRARIES})
target_include_directories(sfml-source PRIVATE ${CMAKE_BINARY_DIR})
