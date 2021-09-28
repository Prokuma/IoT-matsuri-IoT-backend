#ifndef CRUD_HH
#define CRUD_HH
#include <map>
#include <iostream>
#include <vector>
#include <pqxx/pqxx>
#include <boost/optional.hpp>

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
        bool is_to_device;
        std::string device_id;
        std::string message_string;
    };
}


class crud {
    private:
    pqxx::connection conn;
    
    public:
    crud(std::string conn_info) : conn(conn_info){}
    std::string encrypt_message(std::string plain, std::string key);
    std::string decrypt_message(std::string crypted, std::string key);
    boost::optional<models::device> get_device(std::string device_id);
    boost::optional<models::message> get_message(std::string message_id);
    void get_messages_from_device_id(
        std::vector<models::message>& vec,
        std::string device_id
    );
    boost::optional<models::message> create_message(std::string device_id, std::string message);
    void update_device_connection(std::string device_id, bool connection);
    
};


#endif