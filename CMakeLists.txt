cmake_minimum_required(VERSION 3.5.1)
project(BankSystemBackend)

#Set CXX Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#Find required packages
find_package(Threads REQUIRED)
find_package(ODBC REQUIRED)
find_package(CLI11 CONFIG REQUIRED)
find_package(GTest REQUIRED)

#include Crow and nanodbc from /usr/local/include
include_directories(/usr/local/include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

#Add add_subdirectory()
add_subdirectory(src)
add_subdirectory(cpp-dotenv)
#add_subdirectory(tests)

#installation setup
install(TARGETS bank_backend DESTINATION bin)

#find all shared libraries required by the exeutable
set(BANK_BACKEND_EXECUTABLE ${CMAKE_BINARY_DIR}/bin/bank_backend)

#install required shared libraries
set(CPACK_GENERATOR "TGZ;DEB;RPM")
set(CPACK_PACKAGE_NAME "BankSystemBackend")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION "Bank System Backend - A secure banking API")
set(CPACK_PACKAGE_VENDOR "ArbitCode")
set(CPACK_PACKAGE_CONTACT "rajaramsharma422@gmail.com")

#Define package dependencies for DEB and RPM
set(CPACK_DEBIAN_PACKAE_DEPENDS "odbc, libboost-all-dev")
set(CPACK_RPM_PACKAGE_REQUIRES "odbc, boost")

#enable CPack packaging
include(CPack)

