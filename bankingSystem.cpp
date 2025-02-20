#include "bank.hpp"
#include<limits>

using namespace std;

enum class Ops
{
  OPEN_ACCOUNT = 1,
  BALANCE_ENQUIRY,
  DEPOSIT,
  WITHDRAW,
  CLOSE_ACCOUNT,
  SHOW_ALL_ACCOUNTS,
  QUIT
};

template <typename T>
void getInput(T &data)
{
  short count = 3;
  while (!(cin >> data)){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (count){
      cout << "\n\tERROR: Invalid Input! left chance: " << count;
      cout << "\nPlease Enter Again: ";
      count--;
    }else{
      cout << "\n\tERROR: Exit!\n";
      exit(0);
    }
  }
}

int main()
{
  Bank b;
  Account acc;
  Ops ops;

  int choice = -1;
  std::string fname = "";
  std::string lname = "";
  long accountNumber = -1;
  float balance = 0;
  float amount = 0;
  cout << "\n----------------------------";
  cout << "\n\tBanking System" << endl;
  cout << "\n----------------------------";
  do
  {
    cout << "\n\tSelect one option below ";
    cout << "\n\t1. Open an Account";
    cout << "\n\t2. Balance Enquiry";
    cout << "\n\t3. Deposit";
    cout << "\n\t4. Withdraw";
    cout << "\n\t5. Close an Account";
    cout << "\n\t6. Show All Accounts";
    cout << "\n\t7. Quit";
    cout << "\n----------------------------";
    cout << "\nEnter your choice(1-7): ";

    getInput(choice);
    ops = static_cast<Ops>(choice);

    switch (ops){
    case Ops::OPEN_ACCOUNT:
      cout << "Enter First Name: ";
      getInput(fname);
      cout << "Enter Last Name: ";
      getInput(lname);
      cout << "Enter initial Balance: ";
      getInput(balance);
      acc = b.openAccount(fname, lname, balance);
      cout << endl
           << "Congratulation Account is Created" << endl;
      cout << acc;
      break;
    case Ops::BALANCE_ENQUIRY:
      cout << "Enter Account Number: ";
      getInput(accountNumber);
      acc = b.balanceEnquiry(accountNumber);
      cout << endl
           << "Your Account Details" << endl;
      cout << acc;
      break;
    case Ops::DEPOSIT:
      cout << "Enter Account Number: ";
      getInput(accountNumber);
      cout << "Enter Balance: ";
      getInput(amount);
      acc = b.deposit(accountNumber, amount);
      cout << endl
           << "Amout is Deposited" << endl;
      cout << acc;
      break;
    case Ops::WITHDRAW:
      cout << "Enter Account Number: ";
      getInput(accountNumber);
      cout << "Enter Balance: ";
      getInput(amount);
      acc = b.withdraw(accountNumber, amount);
      cout << endl
           << "Amount withdrawn" << endl;
      cout << acc;
      break;
    case Ops::CLOSE_ACCOUNT:
      cout << "Enter Account Number: ";
      getInput(accountNumber);
      b.closeAccount(accountNumber);
      break;
    case Ops::SHOW_ALL_ACCOUNTS:
      b.showAllAccounts();
      break;
    case Ops::QUIT:
      cout << "Thank you!";
      break;
    default:
      cout << "\nEnter correct choice: ";
    }
  } while (ops != Ops::QUIT);

  return 0;
}
