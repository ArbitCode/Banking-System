#ifndef TRANSACTION_CONTROLLER_H
#define TRANSACTION_CONTROLLER_H
#include <crow.h>
#include "../services/TransactionService.h"

void setupTransactionRoutes(crow::SimpleApp &app, nanodbc::connection &conn);

#endif
