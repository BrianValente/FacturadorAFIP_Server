//
//  database.cpp
//  Afip
//
//  Created by Brian Valente on 11/19/19.
//
#include <iostream>

#include "database.hpp"

#include "config.hpp"
#include "libraries/mysql+++/mysql+++.h"

using namespace daotk;

mysql::connection * Database::connection = nullptr;

mysql::connection* Database::instance() {
    if (connection == nullptr || !connection->my_conn) {
        connection = new mysql::connection{
            Config::getString("db.server"),
            Config::getString("db.user"),
            Config::getString("db.password"),
            Config::getString("db")
        };
        
        if (!connection || !connection->my_conn) {
            std::cerr << "Can't connect to DB!" << std::endl;
        }
    }
            
    return connection;
}
