#include <boost/asio/spawn.hpp>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/format.hpp>
#include "server.hh"

namespace asio = boost::asio;

int main() {
    std::string conn_info = (
        boost::format("user=%1% password=%2% host=%3% dbname=%4%") %
        std::getenv("POSTGRES_USER") %
        std::getenv("POSTGRES_PASSWORD") %
        std::getenv("POSTGRES_HOST") %
        std::getenv("POSTGRES_DB")
    ).str();

    asio::io_service io_service;
    crud crud(conn_info);
    server server(io_service, crud, 31400);

    server.start();

    io_service.run();
}
