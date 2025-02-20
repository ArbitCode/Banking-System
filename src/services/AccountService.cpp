#include "AccountService.h"
#include <iostream>
#include<vector>

int AccountService::createAccount(nanodbc::connection &conn, const std::string &owner_name, double balance)
{
	try{
		nanodbc::statement stmt(conn);
		nanodbc::prepare(stmt, "INSERT INTO accounts (owner_name, balance) VALUES(?, ?) RETURNING id");
		stmt.bind(0, owner_name.c_str());
		stmt.bind(1, &balance);
		auto result = nanodbc::execute(stmt);
		if(result.next()){
			return result.get<int>(0);
		}
		return -1;
	}
	catch(std::exception& e){
		std::cerr << "Error creating account: " << e.what() << std::endl;
		return false;
	}
}

std::optional<Account> AccountService::getAccountById(nanodbc::connection &conn, int id)
{
	try{
		nanodbc::statement stmt(conn);
		nanodbc::prepare(stmt, "SELECT id, owner_name, balance FROM accounts WHERE id = ?");
		stmt.bind(0, &id);
		auto result = nanodbc::execute(stmt);
		if(result.next()){
			Account acc{result.get<int>(0), result.get<std::string>(1), result.get<double>(2)};
			return acc;
		}
	}
	catch(const std::exception &ex)
	{
		std::cerr << "Error fetching account: " << ex.what() << std::endl;
	}

	return std::nullopt;
}

void AccountService::deleteAccount(nanodbc::connection &conn, int id)
{
	try{
		nanodbc::statement stmt(conn);
		nanodbc::prepare(stmt, "DELETE FROM accounts where id = ?");
		stmt.bind(0, &id);
		nanodbc::execute(stmt);
	}
	catch(const std::exception &ex){
		std::cerr << "Error deleting account: " << ex.what() << std::endl;
	}
}

std::vector<Account> AccountService::listAccounts(nanodbc::connection &conn)
{
	std::vector<Account> accounts;
	try
	{
		nanodbc::statement stmt(conn);
		nanodbc::prepare(stmt, "SELECT id, owner_name, balance FROM accounts");
		auto result = nanodbc::execute(stmt);
		while (result.next())
		{
			Account acc;
			acc.id = result.get<int>(0);
			acc.owner_name = result.get<std::string>(1);
			acc.balance = result.get<double>(2);
			accounts.push_back(acc);
		}
	}
	catch(std::exception &ex){
		std::cerr << "Error Account List: " << ex.what() << std::endl;
	}
	return accounts;
}

std::vector<Transaction> AccountService::listAccountTransactions(nanodbc::connection &conn, int accountNumber)
{
	std::vector<Transaction> transactions;
	try{
		nanodbc::statement stmt(conn);
		nanodbc::prepare(stmt, "SELECT id, created_at, account_id, amount, transaction_type, remark FROM transactions WHERE account_id = ?");
		stmt.bind(0, &accountNumber);
		auto result = nanodbc::execute(stmt);
		while(result.next()){
			Transaction trans;
			trans.id = result.get<int>(0);
			trans.created_at = result.get<std::string>(1);
			trans.account_id = result.get<int>(2);
			trans.amount = result.get<double>(3);
			trans.transaction_type = result.get<std::string>(4);
			trans.remark = result.get<std::string>(4);
			transactions.push_back(trans);
		}
	}
	catch (std::exception &ex)
	{
		std::cerr << "Error transaction List: " << ex.what() << std::endl;
	}
	return transactions;
}
