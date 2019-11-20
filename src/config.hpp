//
//  config.hpp
//  Afip
//
//  Created by Brian Valente on 11/18/19.
//

#ifndef config_hpp
#define config_hpp

#include <string>

namespace Config {
    std::string getString(std::string);
    int getInteger(std::string);
    unsigned long long getULL(std::string);
}

#endif /* config_hpp */
