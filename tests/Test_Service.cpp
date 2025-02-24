#include <gtest/gtest.h>
#include <nanodbc/nanodbc.h>
#include "dotenv.h"
#include "../src/models/Transaction.h"
#include "../src/models/Account.h"
#include "../src/services/TransactionService.h"
#include "../src/services/AccountService.h"
#include "../src/routes/Routes.h"
#include <vector>
#include<crow.h>
#include<cpr/cpr.h>

class ServiceTest : public ::testing::Test{
protected:
	nanodbc::connection conn;
	crow::SimpleApp app;
	void SetUp() override
	{
		std::string connection_str = "DRIVER={" + dotenv::env["DB_DRIVER"] + "};"
																																				 "SERVER=" +
																 dotenv::env["DB_SERVER"] + ";"
																														"DATABASE=" +
																 dotenv::env["DB_NAME"] + ";"
																													"UID=" +
																 dotenv::env["DB_USER"] + ";"
																													"PWD=" +
																 dotenv::env["DB_PASS"] + ";";
		conn = nanodbc::connection(connection_str);
		setupRoutes(app, conn);
		server_thread = std::thread([this]()
																{ app.port(8080).multithreaded().run(); });
		}
		void TearDown() override
		{
			app.stop();
			conn.disconnect();
			server_thread.join();
		}

	private:
		std::thread server_thread;
};

TEST_F(ServiceTest, CreateAccount){
	int id = AccountService::createAccount(conn, "Dev Tester", 0);
	ASSERT_GT(id, 0);
	AccountService::deleteAccount(conn, id);
}

TEST_F(ServiceTest, GetAcccountById){
	int id = AccountService::createAccount(conn, "Dev Tester", 0);
	auto account = AccountService::getAccountById(conn, id);
	ASSERT_TRUE(account.has_value());
	AccountService::deleteAccount(conn, id);
}

TEST_F(ServiceTest, DeleteAccount){
	int id = AccountService::createAccount(conn, "Dev Tester", 0);
	AccountService::deleteAccount(conn, id);
	auto account = AccountService::getAccountById(conn, id);
	ASSERT_FALSE(account.has_value());
	AccountService::deleteAccount(conn, id);
}

TEST_F(ServiceTest, ListAccount){
	int id1 = AccountService::createAccount(conn, "Dev1", 100);
	int id2 = AccountService::createAccount(conn, "Dev2", 500);
	std::vector<Account> accounts = AccountService::listAccounts(conn);
	ASSERT_FALSE(accounts.empty());
	AccountService::deleteAccount(conn, id1);
	AccountService::deleteAccount(conn, id2);
}

TEST_F(ServiceTest, CreditBalance){
	int id = AccountService::createAccount(conn, "Customer", 0);
	std::string remark = "test_credit";
	auto transaction_id = TransactionService::transaction(conn, id, 1000, remark);
	auto account = AccountService::getAccountById(conn, id);
	ASSERT_EQ(account->balance, 1000);
	AccountService::deleteAccount(conn, id);
}

TEST_F(ServiceTest, WithdrawBalance)
{
	int id = AccountService::createAccount(conn, "Customer", 0);
	std::string remark = "test_credit";
	auto credit_transc_id = TransactionService::transaction(conn, id, 1000, remark);
	auto withdraw_tranc_id = TransactionService::transaction(conn, id, -500, remark);
	auto account = AccountService::getAccountById(conn, id);
	ASSERT_EQ(account->balance, 500);
	AccountService::deleteAccount(conn, id);
}

TEST_F(ServiceTest, ListAccountTransactions)
{
	int id = AccountService::createAccount(conn, "Customer", 0);
	std::string remark = "test_credit";
	auto credit = TransactionService::transaction(conn, id, 1000, remark);
	auto withdraw = TransactionService::transaction(conn, id, -500, remark);
	auto transactions = AccountService::listAccountTransactions(conn, id);
	ASSERT_FALSE(transactions.empty());
	AccountService::deleteAccount(conn, id);
}

TEST_F(ServiceTest, GetTransactionById){
	int id = AccountService::createAccount(conn, "Customer", 0);
	std::string remark = "test_credit";
	auto credit = TransactionService::transaction(conn, id, 1000, remark);
	auto transaction = TransactionService::getTransaction(conn, std::get<Transaction>(credit).id);
	ASSERT_EQ(transaction->id, std::get<Transaction>(credit).id);
	AccountService::deleteAccount(conn, id);
}

TEST_F(ServiceTest, TranferAmount)
{
	int source = AccountService::createAccount(conn, "Customer", 1000);
	int target = AccountService::createAccount(conn, "Customer", 0);
	std::string remark = "test_credit";
	auto transaction = TransactionService::transferAmount(conn, target, source, 500, remark);
	auto sourceAcc = AccountService::getAccountById(conn, source);
	auto targetAcc = AccountService::getAccountById(conn, target);
	ASSERT_EQ(sourceAcc->balance, 500);
	ASSERT_EQ(targetAcc->balance, 500);
	AccountService::deleteAccount(conn, source);
	AccountService::deleteAccount(conn, target);
}

TEST_F(ServiceTest, API_POST_CreateAccount){
	cpr::Response res = cpr::Post(
		cpr::Url{"http://localhost:8080/account"},
		cpr::Header{{"Content-Type", "application/json"}},
		cpr::Body{R"({"owner_name": "Dev", "balance": 1000})"}
	);

	EXPECT_EQ(res.status_code, 201) << "Expected status code 201 but got " << res.status_code;
	
	auto json = crow::json::load(res.text);

	ASSERT_TRUE(json) << "Invalid response: " << res.text;
	EXPECT_EQ(json["status"].s(), "sucess");
	EXPECT_TRUE(json.has("id"));
	EXPECT_GT(json["id"].i(), 0);
}

TEST_F(ServiceTest, API_GET_accountById){
	int id = AccountService::createAccount(conn, "Dev", 500);
	cpr::Response res = cpr::Get(cpr::Url{"http://localhost:8080/account/" + std::to_string(id)});
	EXPECT_EQ(res.status_code, 200);
	auto json = crow::json::load(res.text);
	ASSERT_TRUE(json);
	ASSERT_EQ(json["id"].i(), id);
	AccountService::deleteAccount(conn, id);
}

TEST_F(ServiceTest, API_DELETE_accountById){
	int id = AccountService::createAccount(conn, "Dev", 500);
	cpr::Response res = cpr::Delete(cpr::Url{"http://localhost:8080/account/" + std::to_string(id)});
	EXPECT_EQ(res.status_code, 200);
	cpr::Response res1 = cpr::Get(cpr::Url{"http://localhost:8080/account/" + std::to_string(id)});
	EXPECT_EQ(res1.status_code, 404);
}

TEST_F(ServiceTest, API_GET_allAccounts)
{
	int id1 = AccountService::createAccount(conn, "Dev1", 100);
	int id2 = AccountService::createAccount(conn, "Dev2", 500);

	cpr::Response res = cpr::Get(cpr::Url{"http://localhost:8080/accounts"});
	EXPECT_EQ(res.status_code, 200);

	auto json = crow::json::load(res.text);
	ASSERT_TRUE(json);

	bool found_id1 = false, found_id2 = false;
	for (const auto &account : json)
	{
		ASSERT_TRUE(account.has("id"));
		ASSERT_TRUE(account.has("owner_name"));
		ASSERT_TRUE(account.has("balance"));

		if (account["id"].i() == id1 && account["owner_name"].s() == "Dev1" && account["balance"].i() == 100)
		{
			found_id1 = true;
		}
		if (account["id"].i() == id2 && account["owner_name"].s() == "Dev2" && account["balance"].i() == 500)
		{
			found_id2 = true;
		}
	}
	EXPECT_TRUE(found_id1);
	EXPECT_TRUE(found_id2);

	AccountService::deleteAccount(conn, id1);
	AccountService::deleteAccount(conn, id2);
}

TEST_F(ServiceTest, API_GET_accountTransactions){
	int id = AccountService::createAccount(conn, "dev", 5000);
	std::string remark = "API Test";
	TransactionService::transaction(conn, id, 500, remark);
	TransactionService::transaction(conn, id, -500, remark);
	TransactionService::transaction(conn, id, 500, remark);
	cpr::Response res = cpr::Get(cpr::Url{"http://localhost:8080/account/" + std::to_string(id) + "/transactions"});
	EXPECT_EQ(res.status_code, 200);

	auto json = crow::json::load(res.text);
	ASSERT_TRUE(json);
	int count = 0;
	for (const auto &transaction : json)
	{
		ASSERT_TRUE(transaction.has("id"));
		ASSERT_TRUE(transaction.has("created_at"));
		ASSERT_TRUE(transaction.has("account_id"));
		ASSERT_TRUE(transaction.has("amount"));
		ASSERT_TRUE(transaction.has("transaction_type"));
		ASSERT_TRUE(transaction.has("remark"));

		if (transaction["account_id"].i() == id)
		{
			count++;
		}
	}
	EXPECT_EQ(count, 3);

	AccountService::deleteAccount(conn, id);
}

TEST_F(ServiceTest, API_POST_DepositTransaction)
{
	int id = AccountService::createAccount(conn, "dev", 0);
	cpr::Response res = cpr::Post(
			cpr::Url{"http://localhost:8080/transaction?type=credit"},
			cpr::Header{{"Content-Type", "application/json"}},
			cpr::Body{R"({"account_id": )" + std::to_string(id) +"," + R"("amount": 1000, "remark": "credit"})"});
	auto json = crow::json::load(res.text);
	EXPECT_EQ(res.status_code, 200);
	EXPECT_EQ(json["status"].s(), "sucess");
	AccountService::deleteAccount(conn, id);
}

TEST_F(ServiceTest, API_POST_WithdrawTransaction)
{
	int id = AccountService::createAccount(conn, "dev", 1000);
	cpr::Response res = cpr::Post(
			cpr::Url{"http://localhost:8080/transaction?type=debit"},
			cpr::Header{{"Content-Type", "application/json"}},
			cpr::Body{R"({"account_id": )" + std::to_string(id) + "," + R"("amount": 100, "remark": "credit"})"});
	auto json = crow::json::load(res.text);
	EXPECT_EQ(res.status_code, 200);
	EXPECT_EQ(json["status"].s(), "sucess");
	AccountService::deleteAccount(conn, id);
}

TEST_F(ServiceTest, API_POST_Insufficient)
{
	int id = AccountService::createAccount(conn, "dev", 1000);
	cpr::Response res = cpr::Post(
			cpr::Url{"http://localhost:8080/transaction?type=debit"},
			cpr::Header{{"Content-Type", "application/json"}},
			cpr::Body{R"({"account_id": )" + std::to_string(id) + "," + R"("amount": 1000000, "remark": "credit"})"});
	auto json = crow::json::load(res.text);
	EXPECT_EQ(res.status_code, 400);
	EXPECT_EQ(json["error"].s(), "Insufficient funds!");
	AccountService::deleteAccount(conn, id);
}

TEST_F(ServiceTest, API_Get_TransactionById)
{
	int id = AccountService::createAccount(conn, "dev", 1000);
	cpr::Response res = cpr::Post(
			cpr::Url{"http://localhost:8080/transaction?type=debit"},
			cpr::Header{{"Content-Type", "application/json"}},
			cpr::Body{R"({"account_id": )" + std::to_string(id) + "," + R"("amount": 100, "remark": "credit"})"});
	auto json = crow::json::load(res.text);
	EXPECT_EQ(res.status_code, 200);
	EXPECT_EQ(json["status"].s(), "sucess");
	int transaction_id = json["id"].i();
	cpr::Response res1 = cpr::Get(cpr::Url{"http://localhost:8080/transaction/" + std::to_string(transaction_id)});
	EXPECT_EQ(res1.status_code, 200);
	auto trans = crow::json::load(res1.text);
	EXPECT_EQ(trans["id"].i(), transaction_id);
	AccountService::deleteAccount(conn, id);
}


TEST_F(ServiceTest, API_POST_Transfer){
	int source_id = AccountService::createAccount(conn, "source1", 1000);
	int target_id = AccountService::createAccount(conn, "target", 0);
	cpr::Body body{
			R"({"source_account_id":)" + std::to_string(source_id) +
			R"(,"target_account_id":)" + std::to_string(target_id) +
			R"(,"amount":500})"};

	cpr::Response res = cpr::Post(
			cpr::Url{"http://localhost:8080/transaction/transfer"},
			cpr::Header{{"Content-Type", "application/json"}},
			body);
	auto json = crow::json::load(res.text);
	EXPECT_EQ(res.status_code, 200);
	auto sourceAcc = AccountService::getAccountById(conn, source_id);
	auto targetAcc = AccountService::getAccountById(conn, target_id);
	ASSERT_EQ(sourceAcc->balance, 500);
	ASSERT_EQ(targetAcc->balance, 500);
	AccountService::deleteAccount(conn, source_id);
	AccountService::deleteAccount(conn, target_id);
}

TEST_F(ServiceTest, API_POST_TransferRefund)
{
	int source_id = AccountService::createAccount(conn, "source1", 1000);
	int target_id = AccountService::createAccount(conn, "target", 0);
	cpr::Body body{
			R"({"source_account_id":)" + std::to_string(source_id) +
			R"(,"target_account_id":)" + std::to_string(target_id) + std::to_string(target_id) +
			R"(,"amount":500})"};

	cpr::Response res = cpr::Post(
			cpr::Url{"http://localhost:8080/transaction/transfer"},
			cpr::Header{{"Content-Type", "application/json"}},
			body);
	EXPECT_EQ(res.status_code, 200);
	auto sourceAcc = AccountService::getAccountById(conn, source_id);
	auto targetAcc = AccountService::getAccountById(conn, target_id);
	ASSERT_EQ(sourceAcc->balance, 1000);
	ASSERT_EQ(targetAcc->balance, 0);
	AccountService::deleteAccount(conn, source_id);
	AccountService::deleteAccount(conn, target_id);
}
