#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <boost/asio.hpp>
#include <memory>

using boost::asio::ip::tcp;
namespace asio = boost::asio;

void handleClient(std::shared_ptr<tcp::socket> socket);

class HttpServer {
public:
    HttpServer(asio::io_context& ioContext, unsigned short port, size_t threadPoolSize);
    void run();

private:
    tcp::acceptor acceptor_;
    size_t threadPoolSize_;
};

#endif // HTTPSERVER_H

