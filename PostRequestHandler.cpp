#include "PostRequestHandler.h"
#include "HttpUtil.h"
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

PostRequestHandler::PostRequestHandler(std::shared_ptr<tcp::socket> socket)
    : socket_(socket) {}

void PostRequestHandler::handlePostRequest(const std::string& url, const std::string& body) {
    std::cout << "POST request to " << url << " with body: " << body << std::endl;

    std::string response = httputil::makeResponse(200, "OK", "text/html",
                                                  "<html><body><h1>POST received</h1></body></html>");
    boost::asio::write(*socket_, boost::asio::buffer(response));
}
