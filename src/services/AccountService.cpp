#include "AccountService.h"
#include <iostream>
#include<vector>
#include "../database/QueryRegister.h"
#include <crow/logging.h>

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
		else{
			CROW_LOG_ERROR << "Query Register is nullptr";
		}
	}
	catch(std::exception& ex){
		CROW_LOG_CRITICAL << "Creat account failed: " << ex.what();
	}
	return -1;
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
		else{
			CROW_LOG_ERROR << "Query Register is nullptr";
		}
	}
	catch(const std::exception &ex)
	{
		CROW_LOG_CRITICAL << "Get accunt details failed: " << ex.what();
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
		else
		{
			CROW_LOG_ERROR << "Query Register is nullptr";
		}
	}
	catch(const std::exception &ex){
		CROW_LOG_CRITICAL << "Delete account failed: " << ex.what();
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
		else
		{
			CROW_LOG_ERROR << "Query Register is nullptr";
		}
			}
			catch (std::exception &ex)
			{
				CROW_LOG_CRITICAL << "Account list failed: " << ex.what();
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
		else
		{
			CROW_LOG_ERROR << "Query Register is nullptr";
		}
	}
	catch (std::exception &ex)
	{
		CROW_LOG_CRITICAL << "Transaction List failed: " << ex.what();
	}
	return transactions;
}
