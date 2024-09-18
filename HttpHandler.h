#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include "GetRequestHandler.h"
#include "PostRequestHandler.h"

class HttpHandler {
public:
    HttpHandler(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
    void processRequest();

private:
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    std::string readRequest();
    void sendResponse(const std::string& response);
};

#endif
