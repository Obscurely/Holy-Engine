cmake_minimum_required(VERSION 3.0.0)

project(Holy-Engine)

include(CTest)
enable_testing()

file(GLOB TARGET_SRC "./src/*.cpp" )

add_executable(main ${TARGET_SRC})

find_package(Vulkan REQUIRED FATAL_ERROR)
find_package(glm REQUIRED FATAL_ERROR)
find_package(glfw3 REQUIRED FATAL_ERROR)

target_link_libraries(main vulkan)
target_link_libraries(main glfw)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include(CPack)
