cmake_minimum_required(VERSION 2.8)

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}/include")
set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/shader.cpp
    ${PROJECT_SOURCE_DIR}/src/texture.cpp
    ${PROJECT_SOURCE_DIR}/src/camera.cpp
    ${PROJECT_SOURCE_DIR}/src/mesh.cpp
    ${PROJECT_SOURCE_DIR}/src/model.cpp
    ${PROJECT_SOURCE_DIR}/src/stb_image.cpp
)

add_executable(LearnOpenGL ${SOURCES})

find_package(glbinding REQUIRED)
if (glbinding_FOUND)
    include_directories(${glbinding_INCLUDE_DIR})
endif()
find_package(glfw3 3.2 REQUIRED)
if (glfw3_FOUND)
    include_directories(${glfw3_INCLUDE_DIR})
endif()
find_package(assimp REQUIRED)
if (assimp_FOUND)
    include_directories(${assimp_INCLUDE_DIR})
endif()

target_link_libraries(LearnOpenGL PUBLIC glbinding::glbinding glfw assimp)

