#include "bank.hpp"

Bank::Bank(){
  Account account;
  ifstream infile;
  infile.open("Bank.data");
  if(!infile){
    std::cout << "\nError in Opening! File Not Found!!" << std::endl;
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
  outfile.open("Bank.data", ios::app);
  outfile << account;
  outfile.close();
  return account;
}

Account Bank::balanceEnquiry(long accountNumber){
  auto it = accounts.find(accountNumber);
  if(it != accounts.end()){
    return it->second;  
  }
  throw std::runtime_error("Account Does not exist!");
}

Account Bank::deposit(long accountNumber, float amount){
  auto it = accounts.find(accountNumber);
  if (it != accounts.end())
  {
    it->second.deposit(amount);
    return it->second;
  }
  throw std::runtime_error("Account Does not exist!");
  
}

Account Bank::withdraw(long accountNumber, float amount){
  auto it = accounts.find(accountNumber);
  if (it != accounts.end())
  {
    it->second.withdraw(amount);
    return it->second;
  }
  throw std::runtime_error("Account Does not exist!");
}

void Bank::closeAccount(long accountNumber){
  auto it = accounts.find(accountNumber);
  if (it != accounts.end())
  {
    std::cout << "\nAccount Deleted: " << it->second;
    accounts.erase(accountNumber);
  }
  throw std::runtime_error("Account Does not exist!");
}

void Bank::showAllAccounts()
{
  for(auto it : accounts)
    std::cout << "\nAccount " << it.second << std::endl;
}

Bank::~Bank(){
  ofstream outfile;
  outfile.open("Bank.data", ios::trunc);
  for(auto it :  accounts){
    outfile << it.second;
  }
  outfile.close();
}





























