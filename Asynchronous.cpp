#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

void handle_client(tcp::socket socket, boost::asio::yield_context yield) {
    try {
        char buffer[1024];
        boost::system::error_code ec;
        size_t length = socket.async_read_some(boost::asio::buffer(buffer), yield[ec]);

        if (!ec) {
            std::string response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Length: 13\r\n"
                "Content-Type: text/plain\r\n"
                "\r\n"
                "Hello, World!";

            boost::asio::async_write(socket, boost::asio::buffer(response), yield);
        }
    } catch (std::exception& e) {
        std::cerr << "Client Error: " << e.what() << std::endl;
    }
}

int main() {
    try {
        boost::asio::io_context io;
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 8080));

        boost::asio::spawn(io, [&](boost::asio::yield_context yield) {
            while (true) {
                boost::system::error_code ec;
                tcp::socket socket(io);
                acceptor.async_accept(socket, yield[ec]);

                if (!ec) {
                    boost::asio::spawn(io, 
                        std::bind(handle_client, std::move(socket), std::placeholders::_1));
                }
            }
        });

        std::cout << "Server running on port 8080..." << std::endl;
        io.run();
    } catch (std::exception& e) {
        std::cerr << "Server Error: " << e.what() << std::endl;
    }

    return 0;
}
