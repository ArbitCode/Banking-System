#include "AccountService.h"
#include <iostream>
#include<vector>
#include "../database/QueryRegister.h"

int AccountService::createAccount(nanodbc::connection &conn, const std::string &owner_name, double balance)
{
	try{
		nanodbc::statement stmt(conn);
		auto queryRegister = QueryRegister::get_instance();
		if(queryRegister)
		{
			std::string query = QueryRegister::get_instance()->getQuery("SQL_QUERY_CREATE_ACCOUNT");
			if(!query.empty())
			{
				nanodbc::prepare(stmt, query);
				stmt.bind(0, owner_name.c_str());
				stmt.bind(1, &balance);
				auto result = nanodbc::execute(stmt);
				if (result.next())
				{
					return result.get<int>(0);
				}
			}
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
		auto queryRegister = QueryRegister::get_instance();
		if(queryRegister)
		{
			std::string query = QueryRegister::get_instance()->getQuery("SQL_QUERY_GET_ACCOUNT_BY_ID");
			if(!query.empty())
			{
				nanodbc::prepare(stmt, query);
				stmt.bind(0, &id);
				auto result = nanodbc::execute(stmt);
				if (result.next())
				{
					Account acc{result.get<int>(0), result.get<std::string>(1), result.get<double>(2)};
					return acc;
				}
			}
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
		auto queryRegister = QueryRegister::get_instance();
		if (queryRegister)
		{
			std::string query = QueryRegister::get_instance()->getQuery("SQL_QUERY_DELETE_ACCOUNT_BY_ID");
			if (!query.empty())
			{
				nanodbc::prepare(stmt, query);
				stmt.bind(0, &id);
				nanodbc::execute(stmt);
			}
		}
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
		auto queryRegister = QueryRegister::get_instance();
		if (queryRegister)
		{
			std::string query = QueryRegister::get_instance()->getQuery("SQL_QUERY_GET_ALL_ACCOUNTS");
			if (!query.empty())
			{
				nanodbc::prepare(stmt, query);
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
			}
				
			}
			catch (std::exception &ex)
			{
				std::cerr << "Error Account List: " << ex.what() << std::endl;
			}
			return accounts;
		}

std::vector<Transaction> AccountService::listAccountTransactions(nanodbc::connection &conn, int accountNumber)
{
	std::vector<Transaction> transactions;
	try{
		nanodbc::statement stmt(conn);
		auto queryRegister = QueryRegister::get_instance();
		if (queryRegister)
		{
			std::string query = QueryRegister::get_instance()->getQuery("SQL_QUERY_GET_ALL_ACCOUNT_TRANSACTIONS");
			if (!query.empty())
			{
				nanodbc::prepare(stmt, query);
				stmt.bind(0, &accountNumber);
				auto result = nanodbc::execute(stmt);
				while (result.next())
				{
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
		}
	}
	catch (std::exception &ex)
	{
		std::cerr << "Error transaction List: " << ex.what() << std::endl;
	}
	return transactions;
}
