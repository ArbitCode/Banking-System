#ifndef TRANSACTION_SERVICE_H
#define TRANSACTION_SERVICE_H
#include<nanodbc/nanodbc.h>
#include<optional>
#include "../models/Transaction.h"

class TransactionService{
public:
	static std::variant<Transaction, std::string> transaction(nanodbc::connection &conn, int accountNumber, double amount, std::string &remark);
	static std::optional<Transaction> getTransaction(nanodbc::connection &conn, int transactionId);
	static std::variant<Transaction, std::string> transferAmount(nanodbc::connection &conn, int targetAccountId, int sourceAccountId, double amount, std::string &remark);
};
#endif
