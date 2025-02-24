#include "TransactionService.h"
#include<crow.h>
#include "../database/QueryRegister.h"

std::variant<Transaction, std::string> TransactionService::transaction(nanodbc::connection &conn, int accountNumber, double amount, std::string &remark){
	try{
	nanodbc::statement stmt(conn);
	auto queryRegister = QueryRegister::get_instance();
	if (queryRegister)
	{
		std::string query = QueryRegister::get_instance()->getQuery("SQL_QUERY_PUT_CREDT_DEBIT_TRANSACTION");
		if (!query.empty())
		{
			nanodbc::prepare(stmt, query);
			stmt.bind(0, &amount);
			stmt.bind(1, &amount);
			stmt.bind(2, &amount);
			stmt.bind(3, &accountNumber);
			stmt.bind(4, &amount);
			stmt.bind(5, &accountNumber);
			stmt.bind(6, &amount);
			stmt.bind(7, &amount);
			stmt.bind(8, remark.c_str());

			nanodbc::result response = nanodbc::execute(stmt);
			if (!response.next())
			{
				CROW_LOG_ERROR << "Insufficient fund in source account.";
				return "Insufficient funds!";
			}
			return Transaction{response.get<int>(0), response.get<std::string>(1), response.get<int>(2), response.get<double>(3), response.get<std::string>(4), response.get<std::string>(5)};
		}
	}
	else{
		CROW_LOG_ERROR << "Query Registry is nullptr";
	}
	}
	catch(std::exception &ex){
		CROW_LOG_CRITICAL <<  "Exception transaction failed: " << ex.what();
	}
	return "Transaction Failed.";
}

std::optional<Transaction> TransactionService::getTransaction(nanodbc::connection &conn, int transactionId){
	try{
		nanodbc::statement stmt(conn);
		auto queryRegister = QueryRegister::get_instance();
		if (queryRegister)
		{
			std::string query = QueryRegister::get_instance()->getQuery("SQL_QUERY_GET_TRANSACTION_BY_ID");
			if (!query.empty())
			{
				nanodbc::prepare(stmt, query);
				stmt.bind(0, &transactionId);
				nanodbc::result response = nanodbc::execute(stmt);
				if (response.next())
				{
					return Transaction{response.get<int>(0), response.get<std::string>(1), response.get<int>(2), response.get<double>(3), response.get<std::string>(4), response.get<std::string>(5)};
				}
				else{
					CROW_LOG_ERROR << "Transaction not found.";
				}
			}
		}
		else{
			CROW_LOG_ERROR << "Query Registry is nullptr";
		}
		}
		catch(const std::exception &ex)
		{
			CROW_LOG_CRITICAL <<  "Error fetching tranaction: " << ex.what();
		}

	return std::nullopt;
}
std::variant<Transaction, std::string> TransactionService::transferAmount(nanodbc::connection &conn, int targetAccountId, int sourceAccountId, double amount, std::string &remark)
{
	try{
	//debit : Source Account
	std::variant<Transaction, std::string> result;

	int debit_src_amt = -1.0 * amount;
	std::string debit_remark = remark + ": Tranfer to account_id : " + std::to_string(targetAccountId);
	result = transaction(conn, sourceAccountId, debit_src_amt, debit_remark); //t1
	if(std::holds_alternative<std::string>(result)){
		CROW_LOG_ERROR << "Debit transaction failed remark: " << debit_remark << "Error: " << std::get<std::string>(result);
		return debit_remark + " Failed : " + std::get<std::string>(result); // debit failed.
	}
	//success

	// credit: Target Account
	int credit_target_amt = -1.0 * debit_src_amt;
	std::string credit_remark = remark + ": Received from account_id: " + std::to_string(sourceAccountId);
	result = transaction(conn, targetAccountId, credit_target_amt, credit_remark); //t2
	if (std::holds_alternative<std::string>(result)){
		CROW_LOG_ERROR << "Credit transaction failed remark: " << credit_remark << "Error: " << std::get<std::string>(result);
		//refund = credit source
		std::string refund_remark = remark + ": refund failed for account_id: " + std::to_string(targetAccountId);
		result = transaction(conn, sourceAccountId, credit_target_amt, refund_remark); //t3
		if (std::holds_alternative<std::string>(result))
		{
			CROW_LOG_CRITICAL <<  "Refund transaction failed remark: " << refund_remark << "Error: " << std::get<std::string>(result);
			return refund_remark + " Failed: " + std::get<std::string>(result);
		}
		CROW_LOG_WARNING << "Refund completed.";
	}
	// success
	return std::get<Transaction>(result);
	}
	catch(std::exception &ex){
		CROW_LOG_CRITICAL <<  "Exception tranfer amount failed: " << ex.what();
	}
	return "Transfer failed";
}
