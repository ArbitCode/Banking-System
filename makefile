CC = g++
CFLAGS = -std=c++14

# Targets and rules
all: bankingSystem

bankingSystem: bank.o account.o bankingSystem.o
	$(CC) $(CFLAGS) bin/bank.o bin/account.o bin/bankingSystem.o -o bin/bankingSystem

bankingSystem.o: bankingSystem.cpp
	$(CC) $(CFLAGS) -c bankingSystem.cpp -o bin/bankingSystem.o

bank.o: bank.cpp
	$(CC) $(CFLAGS) -c bank.cpp -o bin/bank.o

account.o: account.cpp
	$(CC) $(CFLAGS) -c account.cpp -o bin/account.o

clean:
	rm bin/*.o bin/bankingSystem

