//
//  print.cpp
//  AFIPServer
//
//  Created by Brian Valente on 11/7/19.
//  Copyright © 2019 Brian Valente. All rights reserved.
//

#include <cstring>
#include <sstream>

#include "printer.hpp"
#include "invoice.hpp"

#define WIDTH  160
#define HEIGHT 500

using namespace std;

namespace Printer {
    int line = 1;

    void add_line(cairo_t *context, const char *str) {
        cairo_move_to(context, 15, 7 * line);
        cairo_show_text(context, str);
        line++;
    }

    int print(Invoice *invoice) {
        string lines[14];
        stringstream stream;
        
        char buf[200];
        char buf2[200];
        char buf3[200];
        char sellingPoint[20], cae[30], invoiceNumber[5], amount[10];

        char* tmpfilename = tempnam(NULL,NULL);
        cairo_surface_t* surface = cairo_ps_surface_create(tmpfilename,
                                                           WIDTH,
                                                           HEIGHT);
        cairo_t *context = cairo_create(surface);

        // draw some text
        cairo_select_font_face(context,
                               "Ticketing",
                               CAIRO_FONT_SLANT_NORMAL,
                               CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(context, 10);
        
        
        memset(buf, 0, sizeof buf);
        memset(buf2, 0, sizeof buf2);
        memset(buf3, 0, sizeof buf3);

        sprintf(sellingPoint, "%d", invoice->sellingPoint);
        sprintf(cae, "%llu", invoice->cae);
        sprintf(invoiceNumber, "%llu", invoice->number);

        lines[0] = "Estética Verónica";
        lines[1] = "CUIT: 23-22650978-5";
        lines[2] = "Ocampo 3365";
        lines[3] = "CP. 1754 - La Matanza";
        lines[4] = "CONSUMIDOR FINAL (99)";
        
        stream << "P.V.: " << invoice->sellingPoint << " Ticket: " << invoice->number;
        lines[5] = stream.str();
        
        stream = stringstream();
        
        lines[6] = "01/01/1990 00:00";
        
        sprintf(amount, "%.2f", invoice->amount);
        stream << "TOTAL: $" << amount;
        lines[8] = stream.str();
        
        stream = stringstream();
        
        
        stream << "Nº CAE: " << cae;
        lines[10] = stream.str();
        
        stream = stringstream();
//
//        lines[11] = "Verifique su factura en";
//        lines[12] = "https://afip.gob.ar/";
        
        lines[7] = "";
        lines[9] = "";
        lines[11] = "";
        lines[12] = "";
        lines[13] = "";

        for (int i = 0; i < 12; i++) {
            add_line(context, lines[i].c_str());
        }

        // finish up
        cairo_show_page(context);
        cairo_destroy(context);
        cairo_surface_flush(surface);
        cairo_surface_destroy(surface);

        // print
        cupsPrintFile(cupsGetDefault(), tmpfilename, string("AFIP Ticket Nº " + to_string(invoice->number)).c_str(), 0, NULL);
        unlink(tmpfilename);

        return 0;
    }
}
