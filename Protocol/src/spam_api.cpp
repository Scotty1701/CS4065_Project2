#include "spam_api.hpp"

Json::Value spam_api::gen_request_connect(std::string address, std::string port) {
    Json::Value message;

    message["message_type"] = "connect";
    message["is_request"] = true;
    message["payload"] = address + ":" + port;

    return message;
}
