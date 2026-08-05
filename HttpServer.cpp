#include "HttpServer.h"
#include "HttpHandler.h"
#include "Logger.h"
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

HttpServer::HttpServer(boost::asio::io_context& ioContext, unsigned short port, size_t threadPoolSize)
    : acceptor_(ioContext, tcp::endpoint(tcp::v4(), port)), threadPoolSize_(threadPoolSize) {}

void HttpServer::run() {
    ThreadPool pool(threadPoolSize_);
    // 用 shared_ptr 持有 logger：任务捕获引用并保证其生命周期覆盖所有请求
    auto logger = std::make_shared<Logger>("server.log");

    for (;;) {
        tcp::socket socket(acceptor_.get_executor());
        acceptor_.accept(socket);
        auto socketPtr = std::make_shared<tcp::socket>(std::move(socket));
        pool.enqueue([socketPtr, logger]() {
            HttpHandler handler(socketPtr, *logger);
            logger->log("New request received from: " + socketPtr->remote_endpoint().address().to_string());
            handler.processRequest();
        });
    }
}
