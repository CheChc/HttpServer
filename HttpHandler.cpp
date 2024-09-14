#include "HttpHandler.h"
#include <iostream>
#include <boost/asio.hpp>
#include "Logger.h"  // 用于记录请求日志

HttpHandler::HttpHandler(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
    : socket_(socket) {}
Logger logger("./log/1.txt");

void HttpHandler::processRequest() {
    try {
        // 读取请求数据
        std::string request = readRequest();
        
        if (request.empty()) return;

        // 记录请求到日志
        logger.log("Received request:\n" + request);
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
        // 处理请求并生成响应
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!";
        
        // 发送响应
        sendResponse(response);
        
        // 记录响应到日志
        logger.log("Sent response:\n" + response);
    } catch (std::exception& e) {
        logger.log("Error processing request: " + std::string(e.what()));
    }
}

std::string HttpHandler::readRequest() {
    boost::asio::streambuf buffer;
    boost::system::error_code error;
    
    // 读取 socket 中的请求数据
    boost::asio::read_until(*socket_, buffer, "\r\n", error);

    if (error && error != boost::asio::error::eof) {
        throw boost::system::system_error(error);
    }

    // 将请求数据转换为字符串
    std::istream requestStream(&buffer);
    std::string request;
    std::getline(requestStream, request);

    return request;
}

void HttpHandler::sendResponse(const std::string& response) {
    boost::asio::write(*socket_, boost::asio::buffer(response));
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