
#ifndef WSFE_H
#define WSFE_H

#include <string>

#include "login.hpp"
#include "soap/soapServiceSoapProxy.h"

//#include "../invoice.hpp"

using namespace std;

class Invoice;

namespace Wsfe {
	Invoice* getLastInvoice();
	bool testConnection(AccessCredentials*);
    bool sendInvoice(Invoice*);
}

#endif
