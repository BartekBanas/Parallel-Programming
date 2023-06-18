cmake_minimum_required(VERSION 3.9)
project(lab10 LANGUAGES  C)

set(CMAKE_C_STANDARD 17)

# Add -O0 to remove optimizations when using gcc
IF(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
ENDIF(CMAKE_COMPILER_IS_GNUCC)


find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(lab10 PUBLIC OpenMP::OpenMP_CXX)
endif()




add_executable(lab10 main.c)

target_link_libraries(lab10 PRIVATE OpenMP::OpenMP_C)
set_property(TARGET lab10 PROPERTY ENVIRONMENT "OMP_NUM_THREADS=5")


add_compile_options(-fopenmp)
add_compile_options($export OMP_NUM_THREADS=10)

if(NOT TARGET OpenMP::OpenMP_CXX)
    find_package(Threads REQUIRED)
    add_library(OpenMP::OpenMP_CXX IMPORTED INTERFACE)
    set_property(TARGET OpenMP::OpenMP_CXX
            PROPERTY INTERFACE_LINK_LIBRARIES ${OpenMP_CXX_FLAGS} Threads::Threads)
    # Only works if the same flag is passed to the linker; use CMake 3.9+ otherwise (Intel, AppleClang)

endif()



#target_compile_options(lab10 PRIVATE export OMP_NUM_THREADS=10)