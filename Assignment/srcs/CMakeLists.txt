cmake_minimum_required(VERSION 3.10)

# Project name
project(RSA_Project)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Manually specify GMP paths
set(GMP_INCLUDE_DIR "/usr/include")
set(GMP_LIBRARIES "/usr/lib/x86_64-linux-gnu/libgmp.so" "/usr/lib/x86_64-linux-gnu/libgmpxx.so")

# Include directories for headers
include_directories(${GMP_INCLUDE_DIR})
include_directories(src)
include_directories(src/Core)
include_directories(src/RSA)
include_directories(src/Utils)

# Enable OpenMP
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    set(OpenMP_LIB OpenMP::OpenMP_CXX)
else()
    set(OpenMP_LIB "")
endif()

# Define the rsa_lib library with all necessary source files
add_library(rsa_lib
    src/Core/PrimeUtils.cpp
    src/RSA/KeyGenerator.cpp
    src/RSA/RSAOperations.cpp    # Add RSAOperations.cpp here
    src/Utils/Logger.cpp
    src/Utils/RandomGenerator.cpp
)

# Link GMP and OpenMP libraries with rsa_lib
target_link_libraries(rsa_lib ${GMP_LIBRARIES} ${OpenMP_LIB})

# Add the main executable and link it with rsa_lib
add_executable(rsa_main src/main.cpp)
target_link_libraries(rsa_main rsa_lib) 
