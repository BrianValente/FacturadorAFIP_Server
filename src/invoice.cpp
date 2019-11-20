//
//  invoice.cpp
//  AFIPServer
//
//  Created by Brian Valente on 11/14/19.
//  Copyright © 2019 Brian Valente. All rights reserved.
//

#include <stdio.h>
#include <time.h>

#include "invoice.hpp"
#include "libraries/mysql+++/mysql+++.h"
#include "printer.hpp"
#include "config.hpp"
#include "database.hpp"

#include "libraries/json/json.hpp"

using namespace daotk;
using nlohmann::json;

bool Invoice::send() {
    return Wsfe::sendInvoice(this);
}

bool Invoice::save() {
    mysql::connection *my = Database::instance();
    stringstream query;
    
    if (!my) {
        cerr << "There was an error connecting to the database." << endl;
        return false;
    }
    
    query << "INSERT INTO invoices (cuit, selling_point, number, cae, sell_type, document_type, document_number, amount, type, currency, currency_exchange_rate, status) VALUES (";
    query << this->cuit << ", ";
    query << this->sellingPoint << ", ";
    query << this->number << ", ";
    query << this->cae << ", ";
    query << this->sellType << ", ";
    query << this->docType << ", ";
    query << this->docNumber << ", ";
    query << this->amount << ", ";
    query << this->type << ", ";
    query << "\"" << this->currency << "\", ";
    query << this->currencyExchangeRate << ", ";
    query << this->status << ")";
    
    string str = query.str();
    
    my->query(str);
    
    return true;
}

void Invoice::print() {
    Printer::print(this);
}

Invoice Invoice::fromJson(nlohmann::json j) {
    Invoice i;
    
    for (auto element : j.items()) {
        std::string k = element.key();
        std::string v = element.value();
        
        if (k == "id")
            i.id = stoi(v);
        else if (k == "cuit")
            i.cuit = stoull(v);
        else if (k == "cuit")
            i.cuit = stoull(v);
        else if (k == "selling_point")
            i.sellingPoint = stoi(v);
        else if (k == "number")
            i.number = stoull(v);
        else if (k == "cae")
            i.cae = stoull(v);
        else if (k == "sell_type")
            i.sellType = stoi(v);
        else if (k == "doc_type")
            i.docType = stoi(v);
        else if (k == "doc_number")
            i.docNumber = stoull(v);
        else if (k == "amount")
            i.amount = stod(v);
        else if (k == "type")
            i.type = stoi(v);
        else if (k == "currency")
            i.currency = v;
        else if (k == "currency_exchange_rate")
            i.currencyExchangeRate = stod(v);
        else if (k == "date")
            i.date = v;
        else if (k == "status")
            i.status = stoi(v);
    }
    
    return i;
}

json Invoice::toJson() {
    json j = {
        {"id", this->id},
        {"cuit", this->cuit},
        {"selling_point", this->sellingPoint},
        {"number", this->number},
        {"cae", this->cae},
        {"sell_type", this->sellType},
        {"doc_type", this->docType},
        {"doc_number", this->docNumber},
        {"amount", this->amount},
        {"type", this->type},
        {"currency", this->currency},
        {"currency_exchange_rate", this->currencyExchangeRate},
        {"date", this->date},
        {"status", this->status},
    };
    
    return j;
}

Invoice Invoice::fromDb(daotk::mysql::result *res) {
    Invoice i;
    std::string s;
    res->fetch(
              i.id,
              i.cuit,
              i.sellingPoint,
              i.number,
              i.cae,
              i.sellType,
              i.docType,
              i.docNumber,
              i.amount,
              i.type,
              i.currency,
              i.currencyExchangeRate,
              i.date,
              s,
              s,
              i.status
              );
    
    return i;
}


const string getCurrentDateAfipFormat() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y%m%d", &tstruct);

    return buf;
}
