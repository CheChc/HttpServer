#include "HttpHandler.h"
#include <iostream>
#include <sstream>
#include <fstream>

HttpHandler::HttpHandler(std::shared_ptr<tcp::socket> socket) : socket_(socket) {}

std::string HttpHandler::readRequest() {
    boost::system::error_code error;
    char data[1024];
    size_t length = socket_->read_some(boost::asio::buffer(data), error);

    if (error == boost::asio::error::eof) {
        return ""; // Connection closed
    } else if (error) {
        throw boost::system::system_error(error); // Other error
    }

    return std::string(data, length);
}

void HttpHandler::processRequest() {
    std::string request = readRequest();
    
    if (request.empty()) return;

    // Parse the request (e.g., "GET /index.html HTTP/1.1")
    std::istringstream requestStream(request);
    std::string method, url, httpVersion;
    requestStream >> method >> url >> httpVersion;

    if (method == "GET") {
        handleGetRequest(url);
    } else if (method == "POST") {
        // Extract body after the header
        std::string body = request.substr(request.find("\r\n\r\n") + 4);
        handlePostRequest(url, body);
    }
}

void HttpHandler::handleGetRequest(const std::string& url) {
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
        // If the URL doesn't match, return 404
        htmlContent = "<html><body><h1>404 Not Found</h1></body></html>";
    }

    std::string response = generateResponse(htmlContent);
    boost::asio::write(*socket_, boost::asio::buffer(response));
}

void HttpHandler::handlePostRequest(const std::string& url, const std::string& body) {
    std::cout << "POST request to " << url << " with body: " << body << std::endl;

    std::string response = generateResponse("<html><body><h1>POST received</h1></body></html>");
    boost::asio::write(*socket_, boost::asio::buffer(response));
}

std::string HttpHandler::readHtmlFromFile(const std::string& filePath) {
    std::ifstream htmlFile(filePath);
    if (!htmlFile.is_open()) {
        return "<html><body><h1>404 Not Found</h1></body></html>";
    }

    std::stringstream buffer;
    buffer << htmlFile.rdbuf();
    return buffer.str();
}

std::string HttpHandler::generateResponse(const std::string& body) {
    return
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + std::to_string(body.size()) + "\r\n"
        "\r\n" +
        body;
}
