#include "spam_api.hpp"

std::string spam_api::gen_request_connect(std::string address, std::string port) {
    Json::Value message;

    message["message_type"] = "connect";
    message["is_request"] = true;
    message["payload"].append(address);
    message["payload"].append(port);

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen_respond_connect(bool success) {
    Json::Value message;

    message["message_type"] = "connect";
    message["is_request"] = false;
    message["payload"] = success;

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen_request_join(std::string username) {
    Json::Value message;

    message["message_type"] = "join";
    message["is_request"] = true;
    message["payload"] = username;

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen_respond_join(bool success) {
    Json::Value message;

    message["message_type"] = "join";
    message["is_request"] = false;
    message["payload"] = success;

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen_request_post(std::string message_id, std::string sender, std::string post_date, std::string subject, std::string content) {
    Json::Value message;

    message["message_type"] = "post";
    message["is_request"] = true;
    message["payload"].append(message_id);
    message["payload"].append(sender);
    message["payload"].append(post_date);
    message["payload"].append(subject);
    message["payload"].append(content);

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen_respond_post(bool success) {
    Json::Value message;

    message["message_type"] = "post";
    message["is_request"] = false;
    message["payload"] = success;

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen_request_message(std::string message_id) {
    Json::Value message;

    message["message_type"] = "message";
    message["is_request"] = true;
    message["payload"] = message_id;

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen_respond_message(std::string message_id, std::string sender, std::string post_date, std::string subject, std::string content) {
    Json::Value message;

    message["message_type"] = "message";
    message["is_request"] = false;
    message["payload"].append(message_id);
    message["payload"].append(sender);
    message["payload"].append(post_date);
    message["payload"].append(subject);
    message["payload"].append(content);

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen_request_leave(std::string username) {
    Json::Value message;

    message["message_type"] = "leave";
    message["is_request"] = true;
    message["payload"] = username;

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen_respond_leave(bool success) {
    Json::Value message;

    message["message_type"] = "leave";
    message["is_request"] = false;
    message["payload"] = success;

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen_request_getusers(std::string group_id) {
    Json::Value message;

    message["message_type"] = "getusers";
    message["is_request"] = true;
    message["payload"] = group_id;

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen_respond_getusers(std::string users[]) {
    Json::Value message;

    message["message_type"] = "join";
    message["is_request"] = false;
    for (int i = 0; i < users->length(); i++) {
        message["payload"].append(users->at(i));
    }

    std::string output = message.toStyledString();

    return output;
}
