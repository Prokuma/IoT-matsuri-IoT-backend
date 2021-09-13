#ifndef LOGGER_HH
#define LOGGER_HH
#include <iostream>

class logger {
    private:
    std::string datetime_string();
    void print_header(std::string datetime, std::string level);

    public:
    void log_info(std::string ip, std::string message);
    void log_debug(std::string ip, std::string message);
    void log_error(std::string ip, std::string message);
    void log_warning(std::string ip, std::string message);
};

#endif