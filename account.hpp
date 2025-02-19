#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <iostream>
#include <fstream>
using namespace std;

#define MIN_BALANCE 500

class InsufficientFunds{};

class Account{
  private:
    long accountNumber;
    std::string firstName;
    std::string lastName;
    float balance;
    static long nextAccountNumber;

  public:
    Account();
    Account(std::string fname, std::string lname, float balance);
    long getAccNo() {return accountNumber;}
    std::string getFirstName(){return firstName;}
    std::string getLastName(){return lastName;}
    float getBalance(){return balance;}

    void deposit(float amount);
    void withdraw(float amount);
    static void setLastAccountNumber(long accountNumber);
    static long getLastAccountNumber();

    friend ofstream &operator<<(ofstream &ofs, Account &acc);
    friend ifstream &operator>>(ifstream &ifs, Account &acc);
    friend ostream &operator<<(ostream &os, Account &acc);
};
#endif
