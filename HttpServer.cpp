#include "HttpServer.h"
#include "HttpHandler.h"
#include "Logger.h"
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

#if BOOST_VERSION >= 107000
#define GET_IO_SERVICE(s) ((boost::asio::io_context&)(s).get_executor().context())
#else
#define GET_IO_SERVICE(s) ((s).get_io_service())
#endif


HttpServer::HttpServer(boost::asio::io_context& ioContext, unsigned short port, size_t threadPoolSize)
    : acceptor_(ioContext, tcp::endpoint(tcp::v4(), port)), threadPoolSize_(threadPoolSize) {}

void HttpServer::run() {
    ThreadPool pool(threadPoolSize_);
    Logger logger("server.log");  // 创建日志记录器，记录到 server.log 文件

    for (;;) {
        boost::asio::ip::tcp::socket socket(GET_IO_SERVICE(acceptor_));
        acceptor_.accept(socket);
        auto socketPtr = std::make_shared<boost::asio::ip::tcp::socket>(std::move(socket));
        pool.enqueue([socketPtr, &logger]() {
            HttpHandler handler(socketPtr);
            logger.log("New request received from: " + socketPtr->remote_endpoint().address().to_string());
            handler.processRequest();
        });
    }
}
