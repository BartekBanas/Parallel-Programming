cmake_minimum_required(VERSION 3.23)
project(PR_lab13_integral C)

set(CMAKE_C_STANDARD 17)

find_package(OpenMP REQUIRED)

add_executable(PR_lab13_integral main.c)

target_link_libraries(PR_lab13_integral PRIVATE OpenMP::OpenMP_C)