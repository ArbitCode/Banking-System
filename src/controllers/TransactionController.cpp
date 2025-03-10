#include "TransactionController.h"
#include "../services/TransactionService.h"

void setupTransactionRoutes(crow::SimpleApp &app, nanodbc::connection &conn) {
	CROW_ROUTE(app, "/transaction").methods(crow::HTTPMethod::POST)([&conn](const crow::request &req)
																																	{
		std::string type_params = req.url_params.get("type");
		if (type_params.empty() || (type_params != "credit" && type_params != "debit"))
		{
			CROW_LOG_ERROR << "Invalid type URL params.";
			return crow::response(400, R"({"error": "Invalid type request"})");
		}
		auto body = crow::json::load(req.body);
		if(!body || !body.has("account_id") || !body.has("amount")){
			CROW_LOG_ERROR << "Invalid body request";
			return crow::response(400, R"({"error": "Invalid request"})");
		}
		std::string remark("");
		int account_id = body["account_id"].i();
		double amount = body["amount"].d();
		if(amount <= 0){
			CROW_LOG_ERROR << "Credit/Debit amount can not be less or equal to zero(0)";
			return crow::response(400, R"({"error": "Invalid amount value"})");
		}
		amount = (type_params == "debit") ? (-1.0 * amount) : amount;
		if (body.has("remark"))
		{
			remark = body["remark"].s();
		}

		auto result = TransactionService::transaction(conn, account_id, amount, remark);
		if(std::holds_alternative<std::string>(result)){
			CROW_LOG_ERROR << "Transaction failed error: " << std::get<std::string>(result);
			return crow::response(400, R"({"error": ")" + std::get<std::string>(result) + R"("})");
		}
		const auto transaction = std::get<Transaction>(result);
			crow::json::wvalue response;
			response["status"] = "sucess";
			response["id"] = transaction.id;
			response["account_id"] = transaction.account_id;
			response["amount"] = transaction.amount;
			response["transaction_type"] = transaction.transaction_type;
			response["remark"] = transaction.remark;
			return crow::response(200, response); });

	CROW_ROUTE(app, "/transaction/<int>")
			.methods(crow::HTTPMethod::GET)([&conn](int id){
		
		auto trans = TransactionService::getTransaction(conn, id);
		if (trans)
		{
			crow::json::wvalue trans_json;
			trans_json["id"] = trans->id;
			trans_json["created_at"] = trans->created_at;
			trans_json["account_id"] = trans->account_id;
			trans_json["amount"] = trans->amount;
			trans_json["transaction_type"] = trans->transaction_type;
			trans_json["remark"] = trans->remark;
			return crow::response(200, trans_json);
		}
		CROW_LOG_ERROR << "Transaction not found.";
		return crow::response(404, R"({"error": "Transaction not found."})"); });

	CROW_ROUTE(app, "/transaction/transfer").methods(crow::HTTPMethod::POST)([&conn](const crow::request &req)
																																					 {
		// src account, target account, amount, remark
		auto body = crow::json::load(req.body);
		if (!body || !body.has("source_account_id") || !body.has("target_account_id") || !body.has("amount"))
		{
			CROW_LOG_ERROR << "invalid body request";
			return crow::response(400, R"({"error": "Invalid request"})");
		}
		std::string remark("");
		if (body.has("remark"))
		{
			remark = body["remark"].s();
		}
		int sourceAccountId = body["source_account_id"].i();
		int targetAccountId = body["target_account_id"].i();
		double amount = body["amount"].d();
		if(amount <= 0)
		{
			CROW_LOG_ERROR << "Transfer amount can not be equal or less then zero(0)";
			return crow::response(400, R"({"error": "Invalid amount value"})");
		}
		auto result = TransactionService::transferAmount(conn, targetAccountId, sourceAccountId, amount, remark); 
		if(std::holds_alternative<std::string>(result)){
			CROW_LOG_ERROR << "trander failed: " << std::get<std::string>(result);
			return crow::response(400, R"({"error: ")" + std::get<std::string>(result) + R"("})");
		}
		const auto transaction = std::get<Transaction>(result);
			crow::json::wvalue response;
			response["status"] = "sucess";
			response["id"] = transaction.id;
			response["account_id"] = transaction.account_id;
			response["amount"] = transaction.amount;
			response["transaction_type"] = transaction.transaction_type;
			response["remark"] = transaction.remark;
			return crow::response(200, response); });
}
