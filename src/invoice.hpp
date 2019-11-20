//
//  invoice.hpp
//  AFIPServer
//
//  Created by Brian Valente on 11/14/19.
//  Copyright © 2019 Brian Valente. All rights reserved.
//

#ifndef invoice_h
#define invoice_h

#include "afip/wsfe.hpp"
#include "libraries/json/json.hpp"
#include "libraries/mysql+++/mysql+++.h"
#include "config.hpp"

const std::string getCurrentDateAfipFormat();

class Invoice {
public:
    static const int SELLTYPE_GOODS = 1;
    static const int SELLTYPE_SERVICES = 2;
    static const int SELLTYPE_GOODS_AND_SERVICES = 3;
    
    static const int TYPE_FACTURA_C = 11;
    
    static const int DOCTYPE_FINAL_CONSUMER = 99;
    
    static const int STATUS_UNSENT = 0;
    static const int STATUS_SENT = 1;
    static const int STATUS_ERROR = 2;
    
    double amount = 0, currencyExchangeRate = 1;
    
    unsigned long long
        number = 0,
        cae = 0,
        cuit = Config::getULL("cuit"),
        docNumber = 0;
    
    int
        id = 0,
        docType = Invoice::DOCTYPE_FINAL_CONSUMER,
        sellingPoint = 2,
        type = Invoice::TYPE_FACTURA_C,
        sellType = Invoice::SELLTYPE_SERVICES,
        status = Invoice::STATUS_UNSENT;
    
    std::string date = getCurrentDateAfipFormat(), currency = "PES";

    bool send();
    bool save();
    void print();
    nlohmann::json toJson();
    static Invoice fromJson(nlohmann::json j);
    static Invoice fromDb(daotk::mysql::result *res);
};

#endif /* invoice_h */
