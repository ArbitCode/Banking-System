#include <crow.h>
#include <nanodbc/nanodbc.h>
#include "./routes/Routes.h"
#include <CLI/CLI.hpp>


int main(int argc, char** argv){
	CLI::App cliApp{"Bank System Backend - A secure banking API"};

	int d = 20;
	cliApp.add_option("-d", d, "Debug range");

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
	crow::logger::setLogLevel(logLevel);

	crow::SimpleApp app;

	try
	{
		nanodbc::connection conn("DSN=PostgreSQL;UID=myuser;PWD=mypassword;");
		setupRoutes(app, conn);
		CROW_LOG_INFO << "Server running on http://localhost:8080";
		app.port(8080).multithreaded().run();
	}
	catch(const std::exception& ex)
	{
		CROW_LOG_ERROR << "Server Start Failed: " << ex.what();
	}
}
