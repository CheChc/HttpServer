#include <boost/asio.hpp>
#include "HttpServer.h"
#include<iostream>
int main() {
    try {
        asio::io_context ioContext;
        HttpServer server(ioContext, 8080, 4);
        server.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

