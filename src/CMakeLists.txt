cmake_minimum_required(VERSION 3.12)
project(BlockchainMessaging VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)

# Set source files
set(SOURCES
    src/core/block.cpp
    src/core/blockchain.cpp
    src/core/transaction.cpp
    src/crypto/encryption.cpp
    src/crypto/hash.cpp
    src/crypto/hash_tree.cpp
    src/network/node.cpp
    src/network/p2p_network.cpp
    src/validation/validator.cpp
    src/validation/consensus.cpp
    src/wallet/wallet.cpp
)

# Create library instead of executable
add_library(${PROJECT_NAME}_lib STATIC ${SOURCES})

# Create executable
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)

# Include directories
target_include_directories(${PROJECT_NAME}_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${OPENSSL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}_lib PUBLIC
    ${OPENSSL_LIBRARIES}
    Threads::Threads
    ${Boost_LIBRARIES}
)

# Testing configuration
enable_testing()
add_subdirectory(tests)

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
) 