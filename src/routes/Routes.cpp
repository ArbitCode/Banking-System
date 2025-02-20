#include "Routes.h"
#include <crow.h>
#include "../controllers/AccountController.h"
#include "../controllers/TransactionController.h"

void setupRoutes(crow::SimpleApp &app, nanodbc::connection& conn){
	setupAccountRoutes(app, conn);
	setupTransactionRoutes(app, conn);
}
