#ifndef ACCOUNT_CONTROLLER_H
#define ACCOUNT_CONTROLLER_H
#include <crow.h>
#include "../services/AccountService.h"

void setupAccountRoutes(crow::SimpleApp &app, nanodbc::connection &conn);

#endif
