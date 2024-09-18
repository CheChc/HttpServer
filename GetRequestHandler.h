#ifndef GETREQUESTHANDLER_H
#define GETREQUESTHANDLER_H

#include <string>
#include <boost/asio.hpp>

class GetRequestHandler {
public:
    GetRequestHandler(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
    void handleGetRequest(const std::string& url);

private:
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    std::string readHtmlFromFile(const std::string& filePath);
    std::string generateResponse(const std::string& body);
};

#endif
