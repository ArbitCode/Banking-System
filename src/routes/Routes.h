
#ifndef ROUTES_H
#define ROUTES_H
#include <crow.h>
#include <nanodbc/nanodbc.h>
void setupRoutes(crow::SimpleApp &app, nanodbc::connection &conn);
#endif
