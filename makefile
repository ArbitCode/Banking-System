CC = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I/usr/local/include
LDFLAGS =  -lnanodbc -lodbc
BUILD_DIR = build
TARGET = bankingSystem

SRC = bankingSystem.cpp \
			bank.cpp \
			account.cpp
# Targets and rules
all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET): ${SRC}
	$(CC) $(CXXFLAGS) -o $(BUILD_DIR)/$(TARGET) $(SRC) $(LDFLAGS)
clean:
	rm -rf $(BUILD_DIR)
