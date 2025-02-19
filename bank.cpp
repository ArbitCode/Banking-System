#include "bank.hpp"

Bank::Bank(){
  Account account;
  ifstream infile;
  infile.open("Bank.data");
  if(!infile){
    std::cout << "Error in Opening! File Not Found!!" << std::endl;
    return;
  }
  while(!infile.eof()){
    infile >> account;
    accounts.insert(pair<long, Account>(account.getAccNo(), account));
  }
  Account::setLastAccountNumber(account.getAccNo());
  infile.close();
}

Account Bank::openAccount(std::string fname, std::string lname, float balance){
  ofstream outfile;
  Account account(fname, lname, balance);
  accounts.insert(pair<long, Account>(account.getAccNo(), account));
  outfile.open("Bank.data", ios::trunc);

  for(auto it : accounts)
  {
    outfile << it.second;
  }

  outfile.close();
  return account;
}

Account Bank::balanceEnquiry(long accountNumber){
  auto it = accounts.find(accountNumber);
  return it->second;
}

Account Bank::deposit(long accountNumber, float amount){
  auto it = accounts.find(accountNumber);
  it->second.deposit(amount);
  return it->second;
}

Account Bank::withdraw(long accountNumber, float amount){
  auto it = accounts.find(accountNumber);
  it->second.withdraw(amount);
  return it->second;
}

void Bank::closeAccount(long accountNumber){
  auto it = accounts.find(accountNumber);
  std::cout << "Account Deleted: " << it->second;
  accounts.erase(accountNumber);
}

void Bank::showAllAccounts()
{
  for(auto it : accounts)
    std::cout << "Account " << it.first << std::endl
              << it.second << std::endl;
}

Bank::~Bank(){
  ofstream outfile;
  outfile.open("Bank.data", ios::trunc);
  for(auto it :  accounts){
    outfile << it.second;
  }
  outfile.close();
}





























