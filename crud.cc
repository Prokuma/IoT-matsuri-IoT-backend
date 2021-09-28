#include "crud.hh"
#include <cryptopp/filters.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/base64.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>

std::string crud::encrypt_message(std::string plain, std::string key) {
    std::string result;
    CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption ecb_encryptor(
        (unsigned char*)key.c_str(),
        CryptoPP::AES::MAX_KEYLENGTH
    );

    auto encryptor = new CryptoPP::StreamTransformationFilter(
        ecb_encryptor,
        new CryptoPP::StringSink(result)
    );
    CryptoPP::StringSource s(plain, true, encryptor);

    return result;
}

std::string crud::decrypt_message(std::string crypted, std::string key) {
    std::string result;
    CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption ecb_decryptor(
        (unsigned char*)key.c_str(),
        CryptoPP::AES::MAX_KEYLENGTH
    );
    auto decryptor = new CryptoPP::StreamTransformationFilter(
        ecb_decryptor,
        new CryptoPP::StringSink(result)
    );
    CryptoPP::StringSource s(crypted, true, decryptor);

    return result;
}

boost::optional<models::device> crud::get_device(std::string device_id) {
    models::device tmp_device;
    pqxx::work txn{ this->conn };
    //pqxx::result r(txn.exec("SELECT secret, connection, user_id FROM Device WHERE id = " + device_id ));
    pqxx::result r(txn.exec("SELECT secret, connection, user_id FROM Device WHERE id = \'" + device_id + "\'"));
    
    if (!r.size() || r[0].size() < 3) {
        return boost::none;
    }
   
    tmp_device.id = device_id;
    tmp_device.secret = r[0][0].c_str();
    //tmp_device.connection = r[0][1].c_str();
    tmp_device.connection = *r[0][1].get<bool>();
    tmp_device.user_id = *r[0][2].get<int>();
    
    txn.commit();
    return tmp_device;
}

boost::optional<models::message> crud::get_message(std::string message_id) {
    pqxx::work txn{ this->conn };
    models::message tmp_message;
    pqxx::result r(txn.exec("SELECT device_id, is_to_device, message FROM Message WHERE id = \'" + message_id + "\'"));

    if (!r.size() || r[0].size() < 2) {
        return boost::none;
    }

    tmp_message.id = message_id;
    tmp_message.device_id = r[0][0].c_str();
    tmp_message.is_to_device = *r[0][1].get<bool>();
    tmp_message.message_string = r[0][2].c_str();

    txn.commit();
    return tmp_message;
}

void crud::get_messages_from_device_id(
            std::vector<models::message> &vec,
            std::string device_id) {
    pqxx::work txn{ this->conn };
    pqxx::result r(txn.exec("SELECT id, is_to_device, message FROM Message WHERE device_id = \'" + device_id + "\'"));

    for (auto const& row : r) {
        if (row.size() < 3) {
            continue;
        }
        models::message tmp_message;
        tmp_message.id = row[0].c_str();
        tmp_message.is_to_device = *row[1].get<bool>();
        tmp_message.message_string = row[2].c_str();
        tmp_message.device_id = device_id;
        vec.emplace_back(tmp_message);
    }
    txn.commit();
    return;
}

boost::optional<models::message> crud::create_message(std::string device_id, std::string message) {
    models::message tmp_message;
    pqxx::work txn{ this->conn };

    std::string message_uuid = boost::lexical_cast<std::string>(boost::uuids::random_generator()());
    txn.exec0(
        "INSERT INTO Message (id, is_to_device, device_id, message) "
        "VALUES (\'" + message_uuid + "\', FALSE, \'" + pqxx::to_string(device_id) + "\',\'" + message + "\')"
    );

    txn.commit();

    tmp_message.message_string = message;
    tmp_message.device_id = device_id;
    tmp_message.id = message_uuid;

    return tmp_message;
}

void crud::update_device_connection(std::string device_id, bool connection) {

    pqxx::work txn{ this->conn };

    pqxx::result r(txn.exec("SELECT * FROM Device"));

    txn.exec0(
        "UPDATE Device SET connection = " + pqxx::to_string(connection) 
    );
    txn.commit();
    return;
}