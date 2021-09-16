#include "server.hh"

void device_session::handler_AS(const char *data, std::string ip_address) {
    int token_length = 0;
    token_length |= data[2] << 8;
    token_length |= data[3];
    if (token_length > strlen(data)) {
    logger_.log_error(ip_address, "Invalid token length.");
    read();
    return;
    }
    std::string token = std::string(data).substr(4, token_length);
    token_map_[token] = session_id_;
    token_ = token;
}

void device_session::handler_NS(const char *data, std::string ip_address) {

}

void device_session::handler_MS(const char *data, std::string ip_address) {

}