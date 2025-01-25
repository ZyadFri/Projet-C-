cmake_minimum_required(VERSION 3.10)
project(BlockchainProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Find Qt5
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# For Qt's MOC and UIC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Enable testing
enable_testing()
find_package(GTest REQUIRED)

# Include directories
include_directories(include)

# Main application
add_executable(blockchain
    src/main.cpp
    src/Block.cpp
    src/Blockchain.cpp
    src/Transaction.cpp
    src/Cryptography.cpp
    src/GUI/MainWindow.cpp
    src/GUI/TransactionDialog.cpp
)

target_link_libraries(blockchain
    PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
        Qt5::Core
        Qt5::Widgets
)

# Test executable
add_executable(blockchain_tests
    tests/TransactionTests.cpp
    # Add more tests here: e.g. BlockTests.cpp, BlockchainTests.cpp
    src/Block.cpp
    src/Blockchain.cpp
    src/Transaction.cpp
    src/Cryptography.cpp
)

target_link_libraries(blockchain_tests
    PRIVATE
        GTest::GTest
        GTest::Main
        OpenSSL::SSL
        OpenSSL::Crypto
)

add_test(NAME blockchain_tests COMMAND blockchain_tests)
