//
//  xcode.cpp
//  AFIPServer
//
//  Created by Brian Valente on 8/29/19.
//  Copyright © 2019 Brian Valente. All rights reserved.
//

#include <stdsoap2.h>

// Xcode build workaround
void soap_open_logfile(struct soap *soap, int i) {
    return;
}
