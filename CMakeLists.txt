cmake_minimum_required(VERSION 3.10)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(ProjectName "MdIE")
set(Subsystem "CONSOLE")

project("${ProjectName}" VERSION 0.1)
set(CMAKE_CXX_STANDARD 23)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(filters)

add_subdirectory(dependencies)

find_package(OpenGL REQUIRED)

#app executable
add_library("${ProjectName}-lib" STATIC
    #src files
    src/image.cpp
    #header files
    include/image.hpp
    include/align.hpp
)

add_executable("${ProjectName}-img_test"
    #src files
    src/main.cpp
    #header files
)

target_link_libraries("${ProjectName}-lib" PUBLIC
	glfw
    glad
    OpenGL::GL
)

target_include_directories("${ProjectName}-lib" PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/submodules"
)

target_link_libraries("${ProjectName}-img_test" PUBLIC 
    "${ProjectName}-lib"
)

target_include_directories("${ProjectName}-img_test" PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/submodules"
)

set_target_properties("${ProjectName}-img_test" PROPERTIES 
    FOLDER "mdie_tests"
)