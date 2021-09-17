#include "crud.hh"
#include <cryptopp/filters.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/base64.h>

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

models::device crud::get_device(std::string device_id) {
    return models::device();
}

models::message crud::get_message(std::string message_id) {
    return models::message();
}

void crud::get_messages_from_device_id(
            std::map<std::string, std::string> &map,
            std::string device_id) {

}

bool crud::create_message(std::string device_id, std::string message) {
    return false;
}