#ifndef SERVER_HH
#define SERVER_HH
#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/optional.hpp>
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <map>
#include "logger.hh"

namespace asio = boost::asio;
using asio::ip::tcp;

class device_session: public std::enable_shared_from_this<device_session> {
    private:
    tcp::socket socket_;
    asio::streambuf receive_buff_;
    logger logger_;
    std::map<std::string, boost::uuids::uuid>& token_map_;
    boost::uuids::uuid session_id_;
    boost::optional<std::string> token_ = boost::none;

    void read();
    void write(std::string message);
    void handler_AS(const char* data, std::string ip_address);
    void handler_NS(const char* data, std::string ip_address);
    void handler_MS(const char* data, std::string ip_address);

    public:
    device_session(tcp::socket socket, 
    std::map<std::string, boost::uuids::uuid>& token_map,
    boost::uuids::uuid session_id) 
        :socket_(std::move(socket)), token_map_(token_map), 
        session_id_(session_id) {}
    void start();
    void deliver(std::string message);
    boost::optional<std::string> get_token();
};

class server {
    private:
    asio::io_service& io_service_;
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    asio::streambuf receive_buff_;
    std::map<boost::uuids::uuid, std::shared_ptr<device_session>> session_map;
    std::map<std::string, boost::uuids::uuid> token_map;

    boost::optional<int> find_session(std::string token);
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