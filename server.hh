#ifndef SERVER_HH
#define SERVER_HH
#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/optional.hpp>
#include <memory>

namespace asio = boost::asio;
using asio::ip::tcp;

class device_session: public std::enable_shared_from_this<device_session> {
    private:
    tcp::socket socket_;
    asio::streambuf receive_buff_;
    void read();
    void write(std::string message);

    public:
    device_session(tcp::socket socket) 
        :socket_(std::move(socket)) {}
    void start();
    void deliver(std::string message);
};

class server {
    private:
    asio::io_service& io_service_;
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    asio::streambuf receive_buff_;
    void start_accept();
    void on_accept(const boost::system::error_code& error, tcp::socket socket);
    void start_receive();
    void on_receive(const boost::system::error_code& error, size_t bytes_transferred);

    public:
    server(asio::io_service& io_service, int port)
        :io_service_(io_service),
        acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
        socket_(io_service) {}
    void start();

};

#endif