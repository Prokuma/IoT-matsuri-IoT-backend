#include "server.hh"

void device_session::handler_AS(const char *data, std::string ip_address) {
    unsigned int token_length = 0;
    token_length |= data[2] << 8;
    token_length |= data[3];
    if (token_length > strlen(data)) {
        logger_.log_error(ip_address, "Invalid token length.");
        return;
    }
    std::string token = std::string(data).substr(4, token_length);
    
    auto device = crud_.get_device(token);
    if (!device) {
        logger_.log_error(ip_address, "Can not get device info.");
        return;
    }

    unsigned int message_length = 0;
    message_length |= data[token_length + 3] << 24;
    message_length |= data[token_length + 4] << 16;
    message_length |= data[token_length + 5] << 8;
    message_length |= data[token_length + 6];
    if (message_length > strlen(data) - (token_length + 4)) {
        logger_.log_error(ip_address, "Invalid message length.");
        return;
    }

    std::string crypted = std::string(data).substr(token_length + 8, message_length);
    std::string message = crud_.decrypt_message(crypted, (*device).secret);
    if (message == "HELLO") {
        token_map_[token] = session_id_;
        token_ = token;
    } else {
        logger_.log_error(ip_address, "Authentication was failed.");
    }
}

void device_session::handler_NS(const char *data, std::string ip_address) {
    unsigned int token_length = 0;
    token_length |= data[2] << 8;
    token_length |= data[3];
    if (token_length > strlen(data)) {
        logger_.log_error(ip_address, "Invalid token length.");
        return;
    }

    std::string token = std::string(data).substr(4, token_length);
    if (token_ != token) {
        logger_.log_error(ip_address, "Invalid token.");
        return;
    }

    // CRUDの実装変更が必要なためこれ以上進められない
}

void device_session::handler_MS(const char *data, std::string ip_address) {
    unsigned int token_length = 0;
    token_length |= data[2] << 8;
    token_length |= data[3];
    if (token_length > strlen(data)) {
        logger_.log_error(ip_address, "Invalid token length.");
        return;
    }
    std::string token = std::string(data).substr(4, token_length);
    if (token_ != token) {
        logger_.log_error(ip_address, "Invalid token.");
        return;
    }

    auto device = crud_.get_device(token);
    if (!device) {
        logger_.log_error(ip_address, "Can not get device info.");
        return;
    }

    unsigned int message_length = 0;
    message_length |= data[token_length + 3] << 24;
    message_length |= data[token_length + 4] << 16;
    message_length |= data[token_length + 5] << 8;
    message_length |= data[token_length + 6];
    if (message_length > strlen(data) - (token_length + 4)) {
        logger_.log_error(ip_address, "Invalid message length.");
        return;
    }

    std::string crypted = std::string(data).substr(token_length + 8, message_length);
    std::string message = crud_.decrypt_message(crypted, (*device).secret);
    auto result = crud_.create_message(token, message);
    if (!result) {
        logger_.log_error(ip_address, "Can not create message.");
        return;
    }
}