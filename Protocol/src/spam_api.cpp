#include "spam_api.hpp"

std::string spam_api::gen::request::connect(std::string address, std::string port) {
    Json::Value message;

    message["message_type"] = "connect";
    message["is_request"] = true;
    message["payload"].append(address);
    message["payload"].append(port);

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen::respond::connect(bool success) {
    Json::Value message;

    message["message_type"] = "connect";
    message["is_request"] = false;
    message["payload"] = success;

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen::request::join(std::string username) {
    Json::Value message;

    message["message_type"] = "join";
    message["is_request"] = true;
    message["payload"] = username;

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen::respond::join(bool success) {
    Json::Value message;

    message["message_type"] = "join";
    message["is_request"] = false;
    message["payload"] = success;

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen::request::post(std::string message_id, std::string sender, std::string post_date, std::string subject, std::string content) {
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

std::string spam_api::gen::respond::post(bool success) {
    Json::Value message;

    message["message_type"] = "post";
    message["is_request"] = false;
    message["payload"] = success;

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen::request::message(std::string message_id) {
    Json::Value message;

    message["message_type"] = "message";
    message["is_request"] = true;
    message["payload"] = message_id;

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen::respond::message(std::string message_id, std::string sender, std::string post_date, std::string subject, std::string content) {
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

std::string spam_api::gen::request::leave(std::string username) {
    Json::Value message;

    message["message_type"] = "leave";
    message["is_request"] = true;
    message["payload"] = username;

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen::respond::leave(bool success) {
    Json::Value message;

    message["message_type"] = "leave";
    message["is_request"] = false;
    message["payload"] = success;

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen::request::getusers(std::string group_id) {
    Json::Value message;

    message["message_type"] = "getusers";
    message["is_request"] = true;
    message["payload"] = group_id;

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen::respond::getusers(std::string users[]) {
    Json::Value message;

    message["message_type"] = "join";
    message["is_request"] = false;
    for (int i = 0; i < users->length(); i++) {
        message["payload"].append(users->at(i));
    }

    std::string output = message.toStyledString();

    return output;
}
