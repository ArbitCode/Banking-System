CC = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I/usr/local/include
LDFLAGS =  /root/workspace/Banking-System/nanodbc/build/libnanodbc.a -lodbc

TARGET = bankingSystem

SRC = bankingSystem.cpp \
			bank.cpp \
			account.cpp
# Targets and rules
all: $(TARGET)

$(TARGET): ${SRC}
	$(CC) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)
clean:
	rm -f $(TARGET)
