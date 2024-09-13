#include "HttpServer.h"
#include <boost/asio.hpp>
#include<iostream>
int main() {
    try {
        boost::asio::io_context ioContext;
        HttpServer server(ioContext, 8080, 4);
        server.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
