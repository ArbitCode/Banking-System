#include "AccountController.h"

void setupAccountRoutes(crow::SimpleApp &app, nanodbc::connection &conn)
{
	CROW_ROUTE(app, "/account/<int>")
			.methods(crow::HTTPMethod::GET)([&conn](int id)
																			{ auto account = AccountService::getAccountById(conn, id); 
			if(account){
				crow::json::wvalue respose = {
						{"id", account->id},
						{"owner_name", account->owner_name},
						{"balance", account->balance}
				};
				return crow::response(200, respose.dump());
			}
			return crow::response(404, "Account not found"); });

	CROW_ROUTE(app, "/account/<int>")
			.methods(crow::HTTPMethod::DELETE)([&conn](int id){
				AccountService::deleteAccount(conn, id); 
				return crow::response(200, "Account Deleted");});

	CROW_ROUTE(app, "/account")
			.methods(crow::HTTPMethod::POST)([&conn](const crow::request &req)
																			 { auto body = crow::json::load(req.body); 
			if(!body || !body.has("owner_name") || !body.has("balance")){
				return crow::response(400, "Invalid request");
			}
			std::string owner_name = body["owner_name"].s(); 
			double balance = body["balance"].d();
			int id = AccountService::createAccount(conn, owner_name, balance);
			if (id != -1)
			{
				crow::json::wvalue response;
				response["status"] = "sucess";
				response["id"] = id;
				crow::response res(201, response);
				res.set_header("Content-Type", "application/json");
				return res;
			}
			return crow::response(500, "Failed to create account"); });

	CROW_ROUTE(app, "/accounts")
			.methods(crow::HTTPMethod::GET)([&conn]()
																			{ std::vector<Account> accounts = AccountService::listAccounts(conn);
		std::vector<crow::json::wvalue> vec;
		for (const auto &acc : accounts)
		{
			crow::json::wvalue acc_json;
			acc_json["id"] = acc.id;
			acc_json["owner_name"] = acc.owner_name;
			acc_json["balance"] = acc.balance;
			vec.push_back(acc_json);
			}
			crow::json::wvalue final = std::move(vec);
			return crow::response(200, final); });

	CROW_ROUTE(app, "/account/<int>/transactions")
			.methods(crow::HTTPMethod::GET)([&conn](int account_id)
																			{ std::vector<Transaction> transactions = AccountService::listAccountTransactions(conn, account_id);
		std::vector<crow::json::wvalue> vec;
		for (const auto &trans : transactions)
		{
			crow::json::wvalue trans_json;
			trans_json["id"] = trans.id;
			trans_json["created_at"] = trans.created_at;
			trans_json["account_id"] = trans.account_id;
			trans_json["amount"] = trans.amount;
			trans_json["transaction_type"] = trans.transaction_type;
			trans_json["remark"] = trans.remark;
			vec.push_back(trans_json);
			}
			crow::json::wvalue final = std::move(vec);
			return crow::response(200, final); });
}
