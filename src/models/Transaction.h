#ifndef TRANSACTION_MODEL_H
#define TRANSACTION_MODEL_H
#include<string>
struct Transaction{
	int id;
	std::string created_at;
	int account_id;
	double amount;
	std::string transaction_type;
	std::string remark;
};
#endif
