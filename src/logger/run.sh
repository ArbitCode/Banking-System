#!/bin/bash

# Set variables
APP_NAME="boost_log_test"
BUILD_DIR="build"

# Compile with Boost (dynamic linking)
echo "Compiling $APP_NAME..."
g++ -DBOOST_LOG_DYN_LINK -std=c++11 -Wall boost_log_test.cc \
    -lboost_log_setup -lboost_log -pthread -o "$BUILD_DIR/$APP_NAME"
