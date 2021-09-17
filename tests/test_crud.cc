#include "../crud.hh"
#include <iostream>
#include <vector>
#include <string>
#include <boost/format.hpp>

int main() {
    std::string conn_info = (
        boost::format("user=%1% password=%2% host=%3% dbname=%4%") %
        std::getenv("POSTGRES_USER") %
        std::getenv("POSTGRES_PASSWORD") %
        std::getenv("POSTGRES_HOST") %
        std::getenv("POSTGRES_DB")
    ).str();

    std::cout << "try connect to " << conn_info << std::endl;

    crud crud(conn_info);

    boost::optional<models::message> created_message = crud.create_message("12345", "message");
    
    if (created_message) {
        std::cout << "Write Successfully" << std::endl;
    } else {
        std::cerr << "Write failed" << std::endl;
        return 1;
    }

    boost::optional<models::message> read_message = crud.get_message((*created_message).id);

    if (read_message) {
        std::cout << "Read Successfully" << std::endl;
    } else {
        std::cerr << "Read failed" <<std::endl;
        return 1;
    }

    std::vector<models::message> vec;
    int m_count = 0;
    crud.get_messages_from_device_id(vec, (*read_message).device_id);
    for (auto &m : vec) {
        if (m.device_id == (*read_message).device_id) {
            m_count++;
        }
    }

    if (m_count > 0) {
        std::cout << "Get messages from device id is Successfully" << std::endl;
    } else {
        std::cerr << "Get messages from device id is failed" << std::endl;
        return 1;
    }

    return 0;
}