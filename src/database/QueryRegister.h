#ifndef DATABASE_QUERY_REGISTER_H
#define DATABASE_QUERY_REGISTER_H
#include <string>
#include <unordered_map>
#include <mutex>

class QueryRegister{
public:
	static QueryRegister* get_instance()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_instance == nullptr)
		{
			try{
			m_instance = new QueryRegister();
			}
			catch(const std::bad_alloc &ex)
			{
				std::cout << "Memory allocation failed: " << ex.what() << std::endl;
				m_instance = nullptr;
			}
		}
		return m_instance;
	}

	inline std::string getQuery(const std::string& query_name){
		if(m_query_map.find(query_name) == m_query_map.end()){
			return m_default_query;
		}
		return m_query_map[query_name];
	};

private:
	QueryRegister(){
		m_default_query = "";
		m_query_map = {
				/*Account Service Query*/
				{"SQL_QUERY_CREATE_ACCOUNT", "INSERT INTO accounts (owner_name, balance) VALUES(?, ?) RETURNING id"},
				{"SQL_QUERY_GET_ACCOUNT_BY_ID", "SELECT id, owner_name, balance FROM accounts WHERE id = ?"},
				{"SQL_QUERY_DELETE_ACCOUNT_BY_ID", "DELETE FROM accounts where id = ?"},
				{"SQL_QUERY_GET_ALL_ACCOUNTS", "SELECT id, owner_name, balance FROM accounts"},
				{"SQL_QUERY_GET_ALL_ACCOUNT_TRANSACTIONS", "SELECT id, created_at, account_id, amount, transaction_type, remark FROM transactions WHERE account_id = ?"},
				/*Transaction Service Query*/
				{"SQL_QUERY_PUT_CREDT_DEBIT_TRANSACTION", R"(WITH updated AS (
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
                      )"},
				{"SQL_QUERY_GET_TRANSACTION_BY_ID", "SELECT id, created_at, account_id, amount, transaction_type, remark FROM transactions WHERE id = ?"},
			};
	}

	QueryRegister(const QueryRegister &) = delete;
	QueryRegister &operator=(const QueryRegister &) = delete;
	std::unordered_map<std::string, std::string> m_query_map;
	std::string m_default_query;
	static QueryRegister* m_instance;
	static std::mutex m_mutex;
};

inline QueryRegister* QueryRegister::m_instance = nullptr;
inline std::mutex QueryRegister::m_mutex;
#endif
