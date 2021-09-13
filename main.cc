#include <boost/asio/spawn.hpp>
#include <iostream>
#include <boost/asio.hpp>
#include "server.hh"

namespace asio = boost::asio;

int main()
{
    asio::io_service io_service;
    server server(io_service, 31400);

    server.start();

    io_service.run();
}
