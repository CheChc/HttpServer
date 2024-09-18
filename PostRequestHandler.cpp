#include "PostRequestHandler.h"
#include <iostream>
#include <boost/asio.hpp>

PostRequestHandler::PostRequestHandler(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
    : socket_(socket) {}

void PostRequestHandler::handlePostRequest(const std::string& url, const std::string& body) {
    std::cout << "POST request to " << url << " with body: " << body << std::endl;

    std::string response = generateResponse("<html><body><h1>POST received</h1></body></html>");
    boost::asio::write(*socket_, boost::asio::buffer(response));
}

std::string PostRequestHandler::generateResponse(const std::string& body) {
    return
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + std::to_string(body.size()) + "\r\n"
        "\r\n" +
        body;
}
