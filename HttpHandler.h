#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <boost/asio.hpp>
#include <string>
#include <memory>

using boost::asio::ip::tcp;

class HttpHandler {
public:
    HttpHandler(std::shared_ptr<tcp::socket> socket);
    void processRequest();

private:
    std::shared_ptr<tcp::socket> socket_;

    // Handle different HTTP methods
    void handleGetRequest(const std::string& url);
    void handlePostRequest(const std::string& url, const std::string& body);

    // Generate response based on the file content
    std::string readHtmlFromFile(const std::string& filePath);
    std::string generateResponse(const std::string& body);

    std::string readRequest();
};

#endif
