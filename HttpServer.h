#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "ThreadPool.h"
#include "HttpHandler.h"
#include <boost/asio.hpp>
#include <memory>

class HttpServer {
public:
    HttpServer(boost::asio::io_context& ioContext, unsigned short port, size_t threadPoolSize);

    void run();

private:
    boost::asio::ip::tcp::acceptor acceptor_;
    size_t threadPoolSize_;
};

#endif // HTTPSERVER_H
