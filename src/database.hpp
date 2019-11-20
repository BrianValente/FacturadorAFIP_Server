//
//  database.hpp
//  Afip
//
//  Created by Brian Valente on 11/19/19.
//

#ifndef database_hpp
#define database_hpp

#include "libraries/mysql+++/mysql+++.h"

class Database {
    static daotk::mysql::connection *connection;
    
public:
    static daotk::mysql::connection* instance();
};

#endif /* database_hpp */
