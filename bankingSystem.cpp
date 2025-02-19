#include "bank.hpp"

using namespace std;

template <typename T>
bool isInputValid(T &data)
{
  if (cin >> data)
    return true;
  return false;
}

int main()
{
  Bank b;
  Account acc;

  int choice;
  std::string fname, lname;
  long accountNumber;
  float balance;
  float amount;

  cout << "***Banking System***" << endl;
  do
  {
    cout << "\n\tSelect one option below ";
    cout << "\n\t1 Open an Account";
    cout << "\n\t2 Balance Enquiry";
    cout << "\n\t3 Deposit";
    cout << "\n\t4 Withdraw";
    cout << "\n\t5 Close an Account";
    cout << "\n\t6 Show All Accounts";
    cout << "\n\t7 Quit";
    cout << "\nEnter your choice: ";

    if (!isInputValid(choice))
      goto OUTSIDE;

    switch (choice)
    {
    case 1:
      cout << "Enter First Name: ";
      if (!isInputValid(fname))
        goto OUTSIDE;
      cout << "Enter Last Name: ";
      if (!isInputValid(lname))
        goto OUTSIDE;
      cout << "Enter initial Balance: ";
      if (!isInputValid(balance))
        goto OUTSIDE;
      acc = b.openAccount(fname, lname, balance);
      cout << endl
           << "Congratulation Account is Created" << endl;
      cout << acc;
      break;
    case 2:
      cout << "Enter Account Number: ";
      if (!isInputValid(accountNumber))
        goto OUTSIDE;
      acc = b.balanceEnquiry(accountNumber);
      cout << endl
           << "Your Account Details" << endl;
      cout << acc;
      break;
    case 3:
      cout << "Enter Account Number: ";
      if (!isInputValid(accountNumber))
        goto OUTSIDE;
      cout << "Enter Balance: ";
      if (!isInputValid(amount))
        goto OUTSIDE;
      acc = b.deposit(accountNumber, amount);
      cout << endl
           << "Amout is Deposited" << endl;
      cout << acc;
      break;
    case 4:
      cout << "Enter Account Number: ";
      if (!isInputValid(accountNumber))
        goto OUTSIDE;
      cout << "Enter Balance: ";
      if (!isInputValid(amount))
        goto OUTSIDE;
      acc = b.withdraw(accountNumber, amount);
      cout << endl
           << "Amount withdrawn" << endl;
      cout << acc;
      break;
    case 5:
      cout << "Enter Account Number: ";
      if (!isInputValid(accountNumber))
        goto OUTSIDE;
      b.closeAccount(accountNumber);
      break;
    case 6:
      b.showAllAccounts();
      break;
    case 7:
      break;
    default:
      cout << "\nEnter correct choice";
      exit(0);
    }
  } while (choice != 7);
  OUTSIDE:
    cout << "Invalid Input: Please try again\n";
  return 0;
}
