#include "HttpServer.h"
#include "HttpHandler.h"
#include "ThreadPool.h"

#if BOOST_VERSION >= 107000
#define GET_IO_SERVICE(s) ((boost::asio::io_context&)(s).get_executor().context())
#endif

HttpServer::HttpServer(boost::asio::io_context& ioContext, unsigned short port, size_t threadPoolSize)
    : acceptor_(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      threadPoolSize_(threadPoolSize) {}

void HttpServer::run() {
    ThreadPool pool(threadPoolSize_);

    for (;;) {
        boost::asio::ip::tcp::socket socket(GET_IO_SERVICE(acceptor_));
        acceptor_.accept(socket);
        auto socketPtr = std::make_shared<boost::asio::ip::tcp::socket>(std::move(socket));
        
        pool.enqueue([socketPtr]() {
            HttpHandler handler(socketPtr);
            handler.processRequest();
        });
    }
}
