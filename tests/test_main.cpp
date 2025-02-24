#include<gtest/gtest.h>
#include <CLI/CLI.hpp>
#include "dotenv.h"
#include<crow.h>

class TestEnv : public ::testing::Environment{
	public:
	TestEnv(const std::string& envfile){
		dotenv::env.load_dotenv(envfile, false, true);
	}
};
class FileLogHandler : public crow::ILogHandler
{
public:
	void log(std::string message, crow::LogLevel /*level*/) override
	{
		static FILE *log_file = fopen("crow_logs.txt", "a");
		if (log_file)
		{
			fprintf(log_file, "%s\n", message.c_str());
			fflush(log_file);
		}
	}
};

int main(int argc, char **argv){
	::testing::InitGoogleTest(&argc, argv);

	CLI::App cliApp{"Bank System Backend - A secure banking API"};
	std::string env_file = ".env";
	cliApp.add_option("--envfile", env_file, "Environment file path");
	CLI11_PARSE(cliApp, argc, argv);
	::testing::AddGlobalTestEnvironment(new TestEnv(env_file));

	crow::logger::setLogLevel(crow::LogLevel::INFO);
	static FileLogHandler fileLogger;
	crow::logger::setHandler(&fileLogger);

	return RUN_ALL_TESTS();
}
