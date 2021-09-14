#include "server.hh"
#include <boost/asio/buffer.hpp>
#include <boost/asio/completion_condition.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind.hpp>
#include <boost/optional/optional.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <cstddef>
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

boost::optional<std::string> device_session::get_token() {
    return token_;
}

void device_session::read() {
    auto self(shared_from_this());
    asio::async_read(socket_, receive_buff_, asio::transfer_at_least(1),
    [this, self](boost::system::error_code error, std::size_t length){
        std::string ip_address = socket_.remote_endpoint().address().to_string();
        if (!error) {
            const char* data = asio::buffer_cast<const char*>(receive_buff_.data());
            logger_.log_info(ip_address, "Data Received: " + std::string(data));
            
            if (std::string(data).substr(0,2) == "AS") {
                //Authentication Response
                int token_length = 0;
                token_length |= data[2] << 8;
                token_length |= data[3];
                if (token_length > length) {
                    logger_.log_error(ip_address, "Invalid token length.");
                    read();
                    return;
                }
                std::string token = std::string(data).substr(4, token_length);
                token_map_[token] = session_id_;
                token_ = token;
            } else if (std::string(data).substr(0,2) == "NS") {
                //Monitoring Response
            } else if (std::string(data).substr(0,2) == "MS") {
                //Message Response
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
                boost::uuids::uuid session_id = boost::uuids::random_generator{}();
                session_map[session_id] = std::make_shared<device_session>(std::move(socket), token_map, session_id);
                session_map[session_id]->start();
            }
            start_accept();
        }
    );
}