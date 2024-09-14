#include "HttpServer.h"
#include <iostream>
#include <boost/asio.hpp>

int main() {
    try {
        boost::asio::io_context ioContext;
        HttpServer server(ioContext, 8080, 4);  // 端口 8080，线程池大小为 4
        server.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";  // 使用 std::cerr 打印异常信息
    }
    return 0;
}
