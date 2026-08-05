#include "HttpHandler.h"
#include "HttpUtil.h"
#include "Logger.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <algorithm>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

HttpHandler::HttpHandler(std::shared_ptr<tcp::socket> socket, Logger& logger)
    : socket_(socket), logger_(logger) {}

void HttpHandler::processRequest() {
    try {
        std::string request = readRequest();
        if (request.empty())
            return;

        // 只记录请求行，避免把 POST body 写入日志
        std::istringstream requestStream(request);
        std::string method, url, httpVersion;
        requestStream >> method >> url >> httpVersion;
        logger_.log("Received request: " + method + " " + url + " " + httpVersion);

        if (method == "GET") {
            GetRequestHandler getRequestHandler(socket_);
            getRequestHandler.handleGetRequest(url);
        } else if (method == "POST") {
            std::string body = request.substr(request.find("\r\n\r\n") + 4);
            PostRequestHandler postRequestHandler(socket_);
            postRequestHandler.handlePostRequest(url, body);
        } else {
            // 不支持的请求方法
            boost::asio::write(*socket_, boost::asio::buffer(
                httputil::makeResponse(400, "Bad Request", "text/html",
                                       "<html><body><h1>400 Bad Request</h1></body></html>")));
        }

    } catch (std::exception& e) {
        logger_.log("Error processing request: " + std::string(e.what()));
    }
}

std::string HttpHandler::readRequest() {
    boost::asio::streambuf buffer;
    boost::system::error_code error;

    // 读到头部结束（\r\n\r\n），此时 body 可能已部分/全部进入缓冲区
    boost::asio::read_until(*socket_, buffer, "\r\n\r\n", error);
    if (error && error != boost::asio::error::eof)
        throw boost::system::system_error(error);

    std::stringstream header;
    header << std::istream(&buffer).rdbuf();
    std::string request = header.str();

    size_t headerEnd = request.find("\r\n\r\n");
    if (headerEnd == std::string::npos)
        return request;

    // 解析 Content-Length，确定还需读取多少字节的 body
    size_t contentLength = 0;
    std::istringstream headerStream(request.substr(0, headerEnd));
    std::string line;
    while (std::getline(headerStream, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        size_t colon = line.find(':');
        if (colon == std::string::npos)
            continue;
        std::string name = line.substr(0, colon);
        std::transform(name.begin(), name.end(), name.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        if (name == "content-length") {
            contentLength = std::strtoul(line.substr(colon + 1).c_str(), NULL, 10);
        }
    }

    if (contentLength == 0)
        return request;

    // 缓冲区里已收到的 body 可能不全，补齐剩余字节
    size_t buffered = request.size() - (headerEnd + 4);
    if (buffered < contentLength) {
        size_t need = contentLength - buffered;
        boost::asio::read(*socket_, buffer, boost::asio::transfer_exactly(need), error);
        if (error)
            throw boost::system::system_error(error);
        std::string more(need, '\0');
        std::istream(&buffer).read(&more[0], static_cast<std::streamsize>(need));
        request += more;
    }

    return request;
}
