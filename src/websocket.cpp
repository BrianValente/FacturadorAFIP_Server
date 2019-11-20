//
// Copyright (c) 2016-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

//------------------------------------------------------------------------------
//
// Example: WebSocket server, synchronous
//
//------------------------------------------------------------------------------

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <thread>

#include <boost/format.hpp>

#include "libraries/json/json.hpp"
#include "invoice.hpp"
#include "libraries/mysql+++/mysql+++.h"
#include "config.hpp"
#include "database.hpp"

namespace WebSocket {
    using tcp = boost::asio::ip::tcp;               // from <boost/asio/ip/tcp.hpp>
    namespace websocket = boost::beast::websocket; // from <boost/beast/websocket.hpp>
    namespace beast = boost::beast;
    using namespace daotk;
    using nlohmann::json;

    static std::thread* thread;
    static bool stop_signal = false;

    auto str_to_buf(std::string str) {
        return boost::asio::dynamic_buffer(str);
    }

    void ws_write_str(websocket::stream<tcp::socket> *ws, std::string str) {
        auto buf = boost::asio::dynamic_buffer(str);
        ws->write(buf.data());
    }

    void onInvoiceReceived(websocket::stream<tcp::socket> *ws, json *j) {
        Invoice invoice = Invoice::fromJson(*j);
        
        invoice.send();
        invoice.save();
        
        ws_write_str(ws, invoice.toJson().dump(4));
    }

    void onInvoiceListRequestReceived(websocket::stream<tcp::socket> *ws) {
        mysql::connection *my = Database::instance();
        json jArray = json::array();
        
        if (!my) {
            ws_write_str(ws, "There was an error connectio=ng to the DB");
            return;
        }
        
        mysql::result res = my->query("select * from invoices");
        
        while(!res.eof()) {
            Invoice i = Invoice::fromDb(&res);
            jArray.push_back(i.toJson());
            res.next();
        }
        
        ws_write_str(ws, jArray.dump(4));
    }

    void onResponse(websocket::stream<tcp::socket> *ws, std::string str) {
        try {
            json j = json::parse(str);
            
            if (j["action"] == "invoice_create") {
                onInvoiceReceived(ws, &j);
            } else if (j["action"] == "invoice_list") {
                onInvoiceListRequestReceived(ws);
            } else if (j["action"] == "close"){
                ws->close(beast::websocket::close_code::none);
                stop_signal = true;
            } else {
                ws_write_str(ws, "xd");
            }
        } catch (const std::exception e) {
            ws_write_str(ws, "error");
        }
    }

    //------------------------------------------------------------------------------

    // Echoes back all received WebSocket messages
    void
    do_session(tcp::socket& socket)
    {
        std::string ip = socket.remote_endpoint().address().to_string();
        
        std::cout << "Starting connection from IP " << ip << std::endl;
        
        try
        {
            // Construct the stream by moving in the socket
            bool loggedIn = false;
            websocket::stream<tcp::socket> ws{std::move(socket)};

            // Accept the websocket handshake
            ws.accept();

            for(;;)
            {
                // This buffer will hold the incoming message
                boost::beast::multi_buffer buffer;
                
                // Read a message
                ws.read(buffer);

                if (ws.got_text()) {
                    std::string text = beast::buffers_to_string(buffer.data());

                    if (!loggedIn) {
                        loggedIn = text == Config::getString("password");
                        if (!loggedIn) ws_write_str(&ws, "Wrong password!");
                        continue;
                    }
                    
                    onResponse(&ws, text);
                }
                
                //ws.text(ws.got_text());
                

                // Echo the message back
//                ws.text(ws.got_text());
//                ws.write(buffer.data());
            }
        }
        catch(boost::system::system_error const& se)
        {
            // This indicates that the session was closed
            if(se.code() != websocket::error::closed)
                std::cerr << "Error: " << se.code().message() << std::endl;
        }
        catch(std::exception const& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        
        std::cout << "Connection with " << ip << " closed" << std::endl;
    }

    //------------------------------------------------------------------------------

    std::thread* start()
    {
        thread = new std::thread([] {
            try
            {
                auto const address = boost::asio::ip::make_address("0.0.0.0");
                auto const port = static_cast<unsigned short>(Config::getInteger("port"));
                
                std::cout << "Starting websocket server on port " << port << std::endl;

                // The io_context is required for all I/O
                boost::asio::io_context ioc{1};

                // The acceptor receives incoming connections
                tcp::acceptor acceptor{ioc, {address, port}};
                for(;;)
                {
                    if (stop_signal)
                        break;
                    
                    // This will receive the new connection
                    tcp::socket socket{ioc};

                    // Block until we get a connection
                    acceptor.accept(socket);
                    
                    // Launch the session, transferring ownership of the socket
                    std::thread{std::bind(
                        &do_session,
                        std::move(socket))}.detach();
                }
            }
            catch (const std::exception& e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        });
        return thread;
    }

    void stop() {
        // TODO: Implement WebSocket::stop()
    }
}
