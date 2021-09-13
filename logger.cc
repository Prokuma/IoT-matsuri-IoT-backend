#include "logger.hh"
#include <iomanip>
#include <iostream>
#include <ctime>

std::string logger::datetime_string(){
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    char buf[80];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tm);

    return buf;
}

void logger::print_header(std::string datetime, std::string level) {
    std::cout << datetime << " " << "[" << level << "] ";
}

void logger::log_info(std::string ip, std::string message) {
    print_header(datetime_string(), "INFO");
    std::cout << ip << " : " << message << std::endl;
}

void logger::log_debug(std::string ip, std::string message) {
    print_header(datetime_string(), "DEBUG");
    std::cout << ip << " : " << message << std::endl;
}

void logger::log_error(std::string ip, std::string message) {
    print_header(datetime_string(), "ERROR");
    std::cout << ip << " : " << message << std::endl;
}

void logger::log_warning(std::string ip, std::string message) {
    print_header(datetime_string(), "WARNING");
    std::cout << ip << " : " << message << std::endl;
}