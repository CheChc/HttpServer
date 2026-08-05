#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include "GetRequestHandler.h"
#include "PostRequestHandler.h"

class Logger;

class HttpHandler {
public:
    HttpHandler(std::shared_ptr<boost::asio::ip::tcp::socket> socket, Logger& logger);
    void processRequest();

private:
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    Logger& logger_;
    std::string readRequest();
};

#endif
