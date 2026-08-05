#include "GetRequestHandler.h"
#include "HttpUtil.h"
#include <fstream>
#include <sstream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

GetRequestHandler::GetRequestHandler(std::shared_ptr<tcp::socket> socket)
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
        boost::asio::write(*socket_, boost::asio::buffer(httputil::notFound()));
        return;
    }

    std::string response = httputil::makeResponse(200, "OK", "text/html", htmlContent);
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
