cmake_minimum_required(VERSION 3.8)
project(lab10_pde LANGUAGES  C)

set(CMAKE_C_STANDARD 17)
set(OMP_NUM_THREADS 10)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -lm")


find_package(OpenMP REQUIRED)

add_executable(lab10_pde main.c)

target_link_libraries(lab10_pde PRIVATE OpenMP::OpenMP_C)
