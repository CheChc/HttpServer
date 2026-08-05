#ifndef HTTPUTIL_H
#define HTTPUTIL_H

#include <string>

namespace httputil {

inline std::string makeResponse(int statusCode, const std::string& statusText,
                                const std::string& contentType,
                                const std::string& body) {
    return "HTTP/1.1 " + std::to_string(statusCode) + " " + statusText + "\r\n"
           "Content-Type: " + contentType + "\r\n"
           "Content-Length: " + std::to_string(body.size()) + "\r\n"
           "Connection: close\r\n"
           "\r\n" +
           body;
}

inline std::string notFound() {
    const char* body = "<html><body><h1>404 Not Found</h1></body></html>";
    return makeResponse(404, "Not Found", "text/html", body);
}

}  // namespace httputil

#endif  // HTTPUTIL_H