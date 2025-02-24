#include <crow.h>
#include <nanodbc/nanodbc.h>
#include "./routes/Routes.h"
#include <CLI/CLI.hpp>
#include "dotenv.h"

int main(int argc, char** argv){
	CLI::App cliApp{"Bank System Backend - A secure banking API"};

	int d = 20;
	std::string env_file = ".env";
	cliApp.add_option("-d", d, "Debug range");
	cliApp.add_option("--envfile", env_file, "Environment file path");

	CLI11_PARSE(cliApp, argc, argv);

	auto logLevel = crow::LogLevel::INFO;
	switch (d)
	{
	case 10:
		logLevel = crow::LogLevel::DEBUG;
		break;
	case 20:
		logLevel = crow::LogLevel::INFO;
		break;
	case 30:
		logLevel = crow::LogLevel::WARNING;
		break;
	case 40:
		logLevel = crow::LogLevel::ERROR;
		break;
	case 50:
		logLevel = crow::LogLevel::CRITICAL;
		break;
	default:
		logLevel = crow::LogLevel::INFO;
		break;
	}

	auto& dotenv = dotenv::env.load_dotenv(env_file, false, true);

	crow::logger::setLogLevel(logLevel);

	crow::SimpleApp app;

	try
	{
		std::string connection_str = "DRIVER={" + dotenv["DB_DRIVER"] + "};" + "SERVER=" + dotenv["DB_SERVER"] + ";" + "DATABASE=" + dotenv["DB_NAME"] + ";" + "UID=" + dotenv["DB_USER"] + ";" + "PWD=" + dotenv["DB_PASS"] + ";";
		nanodbc::connection conn(connection_str);
		setupRoutes(app, conn);
		CROW_LOG_INFO << "Server running on http://localhost:8080";
		app.port(8080).multithreaded().run();
	}
	catch(const std::exception& ex)
	{
		CROW_LOG_ERROR << "Server Start Failed: " << ex.what();
	}
}
