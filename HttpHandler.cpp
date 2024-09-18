#include "HttpHandler.h"
#include "Logger.h"
#include <iostream>
#include <sstream>
#include <boost/asio.hpp>

HttpHandler::HttpHandler(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
    : socket_(socket) {}

Logger logger("./log/1.txt");

void HttpHandler::processRequest() {
    try {
        std::string request = readRequest();
        if (request.empty()) return;

        logger.log("Received request:\n" + request);
        std::istringstream requestStream(request);
        std::string method, url, httpVersion;
        requestStream >> method >> url >> httpVersion;

        if (method == "GET") {
            GetRequestHandler getRequestHandler(socket_);
            getRequestHandler.handleGetRequest(url);
        } else if (method == "POST") {
            std::string body = request.substr(request.find("\r\n\r\n") + 4);
            PostRequestHandler postRequestHandler(socket_);
            postRequestHandler.handlePostRequest(url, body);
        }

    } catch (std::exception& e) {
        logger.log("Error processing request: " + std::string(e.what()));
    }
}

std::string HttpHandler::readRequest() {
    boost::asio::streambuf buffer;
    boost::system::error_code error;

    boost::asio::read_until(*socket_, buffer, "\r\n", error);

    if (error && error != boost::asio::error::eof) {
        throw boost::system::system_error(error);
    }

    std::istream requestStream(&buffer);
    std::string request;
    std::getline(requestStream, request);

    return request;
}

void HttpHandler::sendResponse(const std::string& response) {
    boost::asio::write(*socket_, boost::asio::buffer(response));
}
