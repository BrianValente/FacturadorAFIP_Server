//
//  websocket.hpp
//  AFIPServer
//
//  Created by Brian Valente on 11/14/19.
//  Copyright © 2019 Brian Valente. All rights reserved.
//

#ifndef websocket_hpp
#define websocket_hpp

#include <stdio.h>
#include <thread>

namespace WebSocket {
    static std::thread* thread;
    std::thread* start();
}

#endif /* websocket_hpp */
