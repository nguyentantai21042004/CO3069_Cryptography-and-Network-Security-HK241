# Add source files for the RSA project
add_library(rsa_lib
    Core/PrimeUtils.cpp
    RSA/KeyGenerator.cpp
    Utils/Logger.cpp
    Utils/RandomGenerator.cpp
)

# Link libraries with GMP for rsa_lib
target_link_libraries(rsa_lib ${GMP_LIBRARIES})

# Link rsa_lib with the main executable
target_link_libraries(rsa_main rsa_lib)
