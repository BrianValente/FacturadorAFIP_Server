//
//  config.cpp
//  Afip
//
//  Created by Brian Valente on 11/18/19.
//

#include <fstream>
#include <string>
#include <sstream>
#include <boost/dll/runtime_symbol_info.hpp>

#include "config.hpp"
#include "libraries/json/json.hpp"

using namespace nlohmann;

namespace Config {
    std::string get(std::string key) {
        std::ifstream stream;
        std::stringstream data;
        std::string binLocation = std::string(boost::dll::program_location().parent_path().string());
        json j;
        
        stream.open(binLocation + "/config.json");
        data << stream.rdbuf();
        
        try {
            j = json::parse(data.str());
            return j[key];
        } catch (std::exception e) {}
        
        return NULL;
    }

    std::string getString(std::string key) {
        return get(key);
    }

    int getInteger(std::string key) {
        return std::stoi(get(key));
    }

    unsigned long long getULL(std::string key) {
        return std::stoull(get(key));
    }
};
