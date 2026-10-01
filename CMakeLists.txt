cmake_minimum_required(VERSION 4.4.3)
project(block_dodge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_ASM_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# set(glm_DIR "C:/Dev/libs/glm/glm-1.0.3/install/lib") # not completed

find_package(glm 1.0.3 CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)

add_executable("${CMAKE_PROJECT_NAME}")

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${SRC_FILES})

target_include_directories("${CMAKE_PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE
    glm::glm
    gdiplus
    magic_enum::magic_enum
)