#include "account.hpp"

Account::Account(){}
Account::Account(std::string fname, std::string lname, float balance){
  nextAccountNumber++;
  accountNumber = nextAccountNumber;
  firstName = fname;
  lastName = lname;
  this->balance = balance;
}

void Account::deposit(float amount){
  balance += amount;
}

void Account::withdraw(float amount){
  if(balance -amount < MIN_BALANCE){
    throw InsufficientFunds();
  }
  balance -= amount;
}

void Account::setLastAccountNumber(long accountNumber){
  nextAccountNumber = accountNumber;
}

long Account::getLastAccountNumber(){
  return nextAccountNumber;
}

ofstream &operator<<(ofstream &ofs, Account &acc){
  ofs<< acc.accountNumber << std::endl;
  ofs << acc.firstName << std::endl;
  ofs << acc.lastName << std::endl;
  ofs << acc.balance << std::endl;
  return ofs;
}

ifstream &operator>>(ifstream &ifs, Account &acc){
  ifs >> acc.accountNumber;
  ifs >> acc.firstName;
  ifs >> acc.lastName;
  ifs >> acc.balance;
  return ifs;
}

ostream &operator<<(ostream &os, Account &acc){
  os << "Account Number: " << acc.getAccNo() << std::endl;
  os << "First Name: " << acc.getFirstName() << std::endl;
  os << "Last Name: " << acc.getLastName() << std::endl;
  os << "Balance: " << acc.getBalance() << std::endl;
  return os;
}

long Account::nextAccountNumber = 0;

































