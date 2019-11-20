//
//  print.hpp
//  AFIPServer
//
//  Created by Brian Valente on 11/7/19.
//  Copyright © 2019 Brian Valente. All rights reserved.
//

#ifndef printer_hpp
#define printer_hpp

#include <stdio.h>
#include <cairo/cairo.h>
#include <cairo/cairo-ps.h>
#include <cups/cups.h>

class Invoice;

namespace Printer {
    int print(Invoice*);
}

#endif /* print_hpp */
