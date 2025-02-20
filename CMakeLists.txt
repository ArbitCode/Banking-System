cmake_minimum_required(VERSION 3.14)
project(BankSystemBackend)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find required packages
find_package(Threads REQUIRED)

# Include Crow and Nanodbc from /usr/local/include
include_directories(/usr/local/include)

# PostgreSQL ODBC driver
find_package(ODBC REQUIRED)

# Define source files
set(SOURCES
    src/main.cpp
    src/routes/Routes.cpp
    src/controllers/AccountController.cpp
    src/controllers/TransactionController.cpp
    src/services/AccountService.cpp
    src/services/TransactionService.cpp
)

# Create the executable
add_executable(bank_backend ${SOURCES})

# Link libraries
target_link_libraries(bank_backend Threads::Threads nanodbc odbc)
