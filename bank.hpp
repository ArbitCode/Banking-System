#ifndef BANK_H
#define BANK_H
using namespace std;
#include<map>
#include "account.hpp"

class Bank
{
  private:
    std::map<long, Account> accounts;

  public:
    Bank();
    ~Bank();
    Account openAccount(std::string fname, std::string lname, float balance);
    Account balanceEnquiry(long accountNumber);
    Account deposit(long accountNumber, float amount);
    Account withdraw(long accountNumber, float amount);
    void closeAccount(long accountNumber);
    void showAllAccounts();
};

#endif
