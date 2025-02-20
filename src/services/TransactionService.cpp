#include "TransactionService.h"
#include<crow.h>

std::variant<Transaction, std::string> TransactionService::transaction(nanodbc::connection &conn, int accountNumber, double amount, std::string &remark){
	// update account
	try{
	std::string qurery =  R"(WITH updated AS (
                            UPDATE accounts
                            SET balance = CASE
                                WHEN ? < 0 AND balance + ? < 0 THEN balance
                                ELSE balance + ?
                                END
                            WHERE id = ?
                            RETURNING id, owner_name, balance, (balance + ? >= 0) AS success
                        ),
                        inserted AS (
                            INSERT INTO transactions (account_id, amount, transaction_type, remark)
                            SELECT ?, ?, CASE WHEN ? >= 0 THEN 'credit' ELSE 'debit' END, ?
                            WHERE EXISTS (SELECT 1 FROM updated WHERE success)
                            RETURNING id, created_at, account_id, amount, transaction_type, remark
                        )
                        SELECT * FROM inserted;
                      )";

	nanodbc::statement stmt(conn);
	nanodbc::prepare(stmt, qurery);
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
		if (!response.next()){
			return "Insufficient funds!";
		}
		return Transaction{response.get<int>(0), response.get<std::string>(1), response.get<int>(2), response.get<double>(3), response.get<std::string>(4), response.get<std::string>(5)};
	}
	catch(std::exception &ex){
		std::cout << "Transaction Error: " << ex.what() << std::endl;
	}
	return "Transaction Failed.";
}

std::optional<Transaction> TransactionService::getTransaction(nanodbc::connection &conn, int transactionId){
	try{
		nanodbc::statement stmt(conn);
		nanodbc::prepare(stmt, "SELECT id, created_at, account_id, amount, transaction_type, remark FROM transactions WHERE id = ?");
		stmt.bind(0, &transactionId);
		nanodbc::result response = nanodbc::execute(stmt);
		if (response.next())
		{
			return Transaction{response.get<int>(0), response.get<std::string>(1), response.get<int>(2), response.get<double>(3), response.get<std::string>(4), response.get<std::string>(5)};
		}
		}
		catch(const std::exception &ex)
		{
			std::cerr << "Error fetching tranaction: " << ex.what() << std::endl;
		}

	return std::nullopt;
}
std::variant<Transaction, std::string> TransactionService::transferAmount(nanodbc::connection &conn, int targetAccountId, int sourceAccountId, double amount, std::string &remark)
{
	//debit : Source Account
	int debit_src_amt = -1.0 * amount;
	std::string debit_remark = remark + "/Tranfer to account_id : " + std::to_string(targetAccountId);
	auto debit_result = transaction(conn, sourceAccountId, debit_src_amt, debit_remark); //t1
	if(std::holds_alternative<std::string>(debit_result)){
		std::cout << "Debit failed: " << debit_remark << std::endl;
		return debit_remark + " Failed : " + std::get<std::string>(debit_result); // debit failed.
	}
	//success
	
	// credit: Target Account
	int credit_target_amt = -1.0 * debit_src_amt;
	std::string credit_remark = remark + "/Received from account_id: " + std::to_string(sourceAccountId);
	auto credit_result = transaction(conn, targetAccountId, credit_target_amt, credit_remark); //t2
	if (std::holds_alternative<std::string>(credit_result)){
		//credit failed
		std::cout << "Credit failed: " << credit_remark << std::endl;
		//refund = credit source
		std::string refund_remark = remark + "/refund of account_id: " + std::to_string(targetAccountId);
		auto refund_result = transaction(conn, sourceAccountId, credit_target_amt, refund_remark); //t3
		if (std::holds_alternative<std::string>(refund_result))
		{
			//refund failed
			std::cout << "refund failed: " << refund_remark << std::endl;
			return refund_remark + " Failed: " + std::get<std::string>(refund_result);
		}
		return "Trafer failed, refund has been completed!";
	}
	// success
	return std::get<Transaction>(debit_result);
}
