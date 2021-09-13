#include "server.hh"
#include <boost/asio/buffer.hpp>
#include <boost/asio/completion_condition.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind.hpp>
#include <boost/optional/optional.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

void device_session::start() {
    write("AQE");
    read();
}

void device_session::deliver(std::string message) {
    write(message);
}

void device_session::read() {
    auto self(shared_from_this());
    asio::async_read(socket_, receive_buff_, asio::transfer_at_least(1),
    [this, self](boost::system::error_code error, std::size_t length){
        std::string ip_address = socket_.remote_endpoint().address().to_string();
        if (!error) {
            const char* data = asio::buffer_cast<const char*>(receive_buff_.data());
            logger_.log_info(ip_address, "Data Received: " + std::string(data));
            
            //Fake Implementations
            //Please implement protocol of IoT Matsuri
            if (std::string(data).substr(0,3) == "REQ") {
                write("RESPONSE\n");
            }
            receive_buff_.consume(length);
            read();
        } else if (error == asio::error::eof) {
            logger_.log_info(ip_address, "EOF received.");
        } else {
            logger_.log_error(ip_address, error.message());
        }
    });
}

void device_session::write(std::string message) {
    auto self(shared_from_this());
    asio::async_write(socket_, asio::buffer(message),
    [this, self](boost::system::error_code error, std::size_t length){
        std::string ip_address = socket_.remote_endpoint().address().to_string();
        if (!error) {
            logger_.log_info(ip_address, "Write Successfully!");
        } else {
            logger_.log_error(ip_address, error.message());
        }
    });
}

void server::start() {
    start_accept();
}

void server::start_accept() {
    acceptor_.async_accept(
        [this](const boost::system::error_code& error, tcp::socket socket) {
            if(!error) {
                std::make_shared<device_session>(std::move(socket))->start();
            }
            start_accept();
        }
    );
}