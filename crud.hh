#ifndef CRUD_HH
#define CRUD_HH
#include <map>
#include <string>

#define BUFSIZE 1024

namespace models {
    struct device {
        std::string id;
        std::string secret;
        int user_id;
        bool connection;
    };

    struct message {
        std::string id;
        std::string device_id;
        std::string message;
    };
}


class crud {
    public:
    models::device get_device(std::string device_id);
    models::message get_message(std::string message_id);
    std::map<std::string, std::string> get_messages_from_device_id(std::string device_id);
    bool create_message(std::string device_id, std::string message);
    std::string encrypt_message(std::string plain, std::string key);
    std::string decrypt_message(std::string crypted, std::string key);
};


#endif