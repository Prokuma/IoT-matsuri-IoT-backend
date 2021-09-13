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
    read();
}

void device_session::deliver(std::string message) {
    write(message);
}

void device_session::read() {
    auto self(shared_from_this());
    asio::async_read(socket_, receive_buff_, asio::transfer_at_least(1),
    [this, self](boost::system::error_code error, std::size_t length){
        if (!error) {
            const char* data = asio::buffer_cast<const char*>(receive_buff_.data());
            std::cout << data << std::endl;
            receive_buff_.consume(length);
            read();
        } else {
            std::cout << "Error Message: " << error.message() << std::endl;
        }
    });
}

void device_session::write(std::string message) {
    auto self(shared_from_this());
    asio::async_write(socket_, asio::buffer(message),
    [this, self](boost::system::error_code error, std::size_t length){
        if (!error) {
            std::cout << "Write Successfully!" << std::endl;
        } else {
            std::cout << error.message() << std::endl;
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