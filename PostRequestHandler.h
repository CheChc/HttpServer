#ifndef POSTREQUESTHANDLER_H
#define POSTREQUESTHANDLER_H

#include <string>
#include <memory>
#include <boost/asio.hpp>

class PostRequestHandler {
public:
    PostRequestHandler(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
    void handlePostRequest(const std::string& url, const std::string& body);

private:
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
};

#endif
