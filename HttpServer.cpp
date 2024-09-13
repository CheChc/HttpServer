#include "HttpServer.h"
#include "ThreadPool.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#if BOOST_VERSION >= 107000
#define GET_IO_SERVICE(s) ((boost::asio::io_context&)(s).get_executor().context())
#endif

std::string readHtmlFromFile(const std::string& filePath) {
    std::ifstream htmlFile(filePath);
    if (!htmlFile.is_open()) {
        return "<html><body><h1>404 Not Found</h1></body></html>";
    }
    
    std::stringstream buffer;
    buffer << htmlFile.rdbuf();
    return buffer.str();
}

void handleClient(std::shared_ptr<tcp::socket> socket) {
    try {
        for (;;) {
            char data[512];
            boost::system::error_code error;
            size_t length = socket->read_some(asio::buffer(data), error);

            if (error == asio::error::eof) {
                break; // Connection closed cleanly by peer.
            } else if (error) {
                throw boost::system::system_error(error); // Some other error.
            }

            // Parse the incoming request
            std::string request(data, length);
            std::cout << "Received request: " << request << std::endl;

            // Extract the URL from the first line (e.g., "GET /index.html HTTP/1.1")
            std::istringstream requestStream(request);
            std::string method, url, httpVersion;
            requestStream >> method >> url >> httpVersion;

            // Choose response based on URL
            std::string htmlContent;
            if (url == "/") {
                htmlContent = readHtmlFromFile("./templates/index.html");
            } else if (url == "/about") {
                htmlContent = readHtmlFromFile("./templates/about.html");
            } else if (url == "/contact") {
                htmlContent = readHtmlFromFile("./templates/contact.html");
            } else {
                htmlContent = "<html><body><h1>404 Not Found</h1></body></html>";
            }

            // Create the HTTP response
            std::string response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: " + std::to_string(htmlContent.size()) + "\r\n"
                "\r\n" +
                htmlContent;

            // Send the response to the client
            asio::write(*socket, asio::buffer(response));
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

HttpServer::HttpServer(asio::io_context& ioContext, unsigned short port, size_t threadPoolSize)
    : acceptor_(ioContext, tcp::endpoint(tcp::v4(), port)), threadPoolSize_(threadPoolSize) {}

void HttpServer::run() {
    ThreadPool pool(threadPoolSize_);

    for (;;) {
        tcp::socket socket(GET_IO_SERVICE(acceptor_));
        acceptor_.accept(socket);
        auto socketPtr = std::make_shared<tcp::socket>(std::move(socket));
        pool.enqueue([socketPtr]() {
            handleClient(socketPtr);
        });
    }
}

