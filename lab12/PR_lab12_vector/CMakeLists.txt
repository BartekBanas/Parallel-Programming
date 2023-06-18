cmake_minimum_required(VERSION 3.23)
project(PR_lab12_vector C)

# Add MPI Package to Project
find_package(MPI REQUIRED)

# Include MPI Headers
include_directories(${MPI_INCLUDE_PATH})

set(CMAKE_C_STANDARD 17)

add_executable(PR_lab12_vector main.c)

# Link with MPI Libraries
target_link_libraries(PR_lab12_vector ${MPI_LIBRARIES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Define Number of Processes to Use
set(NUM_PROCESSES 4)

# Add Custom Target for Running MPI Program
add_custom_target(run_mpi
        COMMAND mpiexec -np ${NUM_PROCESSES} PR_lab12_vector > output
        COMMAND type output
        DEPENDS PR_lab12_vector
        WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
        )

set_property(GLOBAL PROPERTY DEFAULT_TARGET run_mpi)