#include <iostream>
#include <fstream>
#include <thread>

#include "websocket.hpp"
#include "config.hpp"
#include "database.hpp"
#include "afip/login.hpp"

using namespace std;

int main() {
    cout << "Testing connection to database..." << endl;
    auto db = Database::instance();
    if (!db) {
        cerr << "There was an error connecting to the database, exiting...";
        return 1;
    }
    
    cout << "Testing connection to AFIP..." << endl;
    auto creds = Login::getAccessCredentials();
    if (!creds) {
        cerr << "There was an error connecting to AFIP, exiting...";
        return 1;
    }
    
    thread *webSocketThread = WebSocket::start();
    webSocketThread->join();
}
