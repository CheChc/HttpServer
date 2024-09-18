#include "GetRequestHandler.h"
#include <fstream>
#include <sstream>
#include <boost/asio.hpp>

GetRequestHandler::GetRequestHandler(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
    : socket_(socket) {}

void GetRequestHandler::handleGetRequest(const std::string& url) {
    std::string htmlContent;

    // Map URLs to specific HTML files
    if (url == "/") {
        htmlContent = readHtmlFromFile("./templates/index.html");
    } else if (url == "/about") {
        htmlContent = readHtmlFromFile("./templates/about.html");
    } else if (url == "/contact") {
        htmlContent = readHtmlFromFile("./templates/contact.html");
    } else if (url == "/services") {
        htmlContent = readHtmlFromFile("./templates/services.html");
    } else {
        htmlContent = "<html><body><h1>404 Not Found</h1></body></html>";
    }

    std::string response = generateResponse(htmlContent);
    boost::asio::write(*socket_, boost::asio::buffer(response));
}

std::string GetRequestHandler::readHtmlFromFile(const std::string& filePath) {
    std::ifstream htmlFile(filePath);
    if (!htmlFile.is_open()) {
        return "<html><body><h1>404 Not Found</h1></body></html>";
    }

    std::stringstream buffer;
    buffer << htmlFile.rdbuf();
    return buffer.str();
}

std::string GetRequestHandler::generateResponse(const std::string& body) {
    return
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + std::to_string(body.size()) + "\r\n"
        "\r\n" +
        body;
}
