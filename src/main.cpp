#include <crow.h>
#include <nanodbc/nanodbc.h>
#include "./routes/Routes.h"

int main(){
	crow::SimpleApp app;
	try
	{
		nanodbc::connection conn("DSN=PostgreSQL;UID=myuser;PWD=mypassword;");
		setupRoutes(app, conn);
		std::cout
				<< "Server running on http://localhost:8080" << std::endl;
		app.port(8080).multithreaded().run();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Main Error: " << e.what() << '\n';
	}
	
}
