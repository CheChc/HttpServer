#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include <boost/asio.hpp>
#include <memory>
#include "Logger.h"  // 用于记录请求日志

class HttpHandler {
public:
    // 构造函数，接受 socket 共享指针
    HttpHandler(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

    // 处理请求的方法
    void processRequest();

private:
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;

    // 辅助函数，读取 HTTP 请求数据
    std::string readRequest();

    // 辅助函数，发送 HTTP 响应
    void sendResponse(const std::string& response);
    void handleGetRequest(const std::string &url);
    void handlePostRequest(const std::string &url, const std::string &body);
    std::string readHtmlFromFile(const std::string &filePath);
    std::string generateResponse(const std::string &body);
};

#endif
