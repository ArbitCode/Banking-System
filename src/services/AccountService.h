#ifndef ACCOUNT_SERVICE_H
#define ACCOUNT_SERVICE_H

#include <nanodbc/nanodbc.h>
#include <optional>
#include "../models/Account.h"
#include "../models/Transaction.h"

class AccountService{
public:
	static int createAccount(nanodbc::connection &conn, const std::string &owner_name, double balance);
	static std::optional<Account> getAccountById(nanodbc::connection &conn, int id);
	static void deleteAccount(nanodbc::connection &conn, int id);
	static std::vector<Account> listAccounts(nanodbc::connection &conn);
	static std::vector<Transaction> listAccountTransactions(nanodbc::connection &conn, int accountNumber);
};

#endif
