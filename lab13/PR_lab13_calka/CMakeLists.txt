cmake_minimum_required(VERSION 3.23)
project(PR_lab13_calka C)

# Add MPI Package to Project
find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)

# Include MPI Headers
include_directories(${MPI_INCLUDE_PATH})

set(CMAKE_C_STANDARD 17)

add_executable(PR_lab13_calka main.c)

# Link with MPI Libraries
target_link_libraries(PR_lab13_calka ${MPI_LIBRARIES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")

IF(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

# Define Number of Processes to Use
set(NUM_PROCESSES 4)

# Add Custom Target for Running MPI Program
add_custom_target(run_mpi
        COMMAND mpiexec -np ${NUM_PROCESSES} PR_lab13_calka > output
        COMMAND type output
        DEPENDS PR_lab13_calka
        WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
        )

set_property(GLOBAL PROPERTY DEFAULT_TARGET run_mpi)