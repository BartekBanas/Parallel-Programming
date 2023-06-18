cmake_minimum_required(VERSION 3.23)
project(PR_lab12_pthreadsish C)

# Add MPI Package to Project
find_package(MPI REQUIRED)

# Include MPI Headers
include_directories(${MPI_INCLUDE_PATH})

set(CMAKE_C_STANDARD 17)

add_executable(PR_lab12_pthreadsish main.c)

# Link with MPI Libraries
target_link_libraries(PR_lab12_pthreadsish ${MPI_LIBRARIES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Define Number of Processes to Use
set(NUM_PROCESSES 10)

# Add Custom Target for Running MPI Program
add_custom_target(run_mpi
        COMMAND mpiexec -np ${NUM_PROCESSES} PR_lab12_pthreadsish > output
        COMMAND type output
        DEPENDS PR_lab12_pthreadsish
        WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
        )

set_property(GLOBAL PROPERTY DEFAULT_TARGET run_mpi)