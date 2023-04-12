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

std::string spam_api::gen::respond::getusers(std::vector<std::string> users) {
    Json::Value message;

    message["message_type"] = "getusers";
    message["is_request"] = false;
    for (std::string user : users) {
        message["payload"].append(user);
    }

    std::string output = message.toStyledString();

    return output;
}

std::map<std::string, std::string> spam_api::parse::request::connect(std::string& json_message) {
    Json::Reader reader;
    Json::Value message;
    reader.parse(json_message, message);

    std::map<std::string, std::string> output;
    output["message_type"] = message["message_type"].asString();
    output["is_request"] = message["is_request"].asString();
    output["address"] = message["payload"][0].asString();
    output["port"] = message["payload"][1].asString();

    return output;
}

std::map<std::string, std::string> spam_api::parse::request::join(std::string& json_message) {
    Json::Reader reader;
    Json::Value message;
    reader.parse(json_message, message);

    std::map<std::string, std::string> output;
    output["message_type"] = message["message_type"].asString();
    output["is_request"] = message["is_request"].asString();
    output["username"] = message["payload"].asString();

    return output;
}

std::map<std::string, std::string> spam_api::parse::request::post(std::string& json_message) {
    Json::Reader reader;
    Json::Value message;
    reader.parse(json_message, message);

    std::map<std::string, std::string> output;
    output["message_type"] = message["message_type"].asString();
    output["is_request"] = message["is_request"].asString();
    output["message_id"] = message["payload"][0].asString();
    output["sender"] = message["payload"][1].asString();
    output["post_date"] = message["payload"][2].asString();
    output["subject"] = message["payload"][3].asString();
    output["content"] = message["payload"][4].asString();

    return output;
}

std::map<std::string, std::string> spam_api::parse::request::message(std::string& json_message) {
    Json::Reader reader;
    Json::Value message;
    reader.parse(json_message, message);

    std::map<std::string, std::string> output;
    output["message_type"] = message["message_type"].asString();
    output["is_request"] = message["is_request"].asString();
    output["message_id"] = message["payload"].asString();
    return output;
}

std::map<std::string, std::string> spam_api::parse::request::leave(std::string& json_message) {
    Json::Reader reader;
    Json::Value message;
    reader.parse(json_message, message);

    std::map<std::string, std::string> output;
    output["message_type"] = message["message_type"].asString();
    output["is_request"] = message["is_request"].asString();
    output["username"] = message["payload"].asString();
    return output;
}

std::map<std::string, std::string> spam_api::parse::request::getusers(std::string& json_message) {
    Json::Reader reader;
    Json::Value message;
    reader.parse(json_message, message);

    std::map<std::string, std::string> output;
    output["message_type"] = message["message_type"].asString();
    output["is_request"] = message["is_request"].asString();
    output["group_id"] = message["payload"].asString();
    return output;
}

std::map<std::string, std::string> spam_api::parse::respond::connect(std::string& json_message) {
    Json::Reader reader;
    Json::Value message;
    reader.parse(json_message, message);

    std::map<std::string, std::string> output;
    output["message_type"] = message["message_type"].asString();
    output["is_request"] = message["is_request"].asString();
    output["success"] = message["success"].asString();

    return output;
}

std::map<std::string, std::string> spam_api::parse::respond::join(std::string& json_message) {
    Json::Reader reader;
    Json::Value message;
    reader.parse(json_message, message);

    std::map<std::string, std::string> output;
    output["message_type"] = message["message_type"].asString();
    output["is_request"] = message["is_request"].asString();
    output["success"] = message["success"].asString();

    return output;
}

std::map<std::string, std::string> spam_api::parse::respond::post(std::string& json_message) {
    Json::Reader reader;
    Json::Value message;
    reader.parse(json_message, message);

    std::map<std::string, std::string> output;
    output["message_type"] = message["message_type"].asString();
    output["is_request"] = message["is_request"].asString();
    output["success"] = message["success"].asString();

    return output;
}

std::map<std::string, std::string> spam_api::parse::respond::message(std::string& json_message) {
    Json::Reader reader;
    Json::Value message;
    reader.parse(json_message, message);

    std::map<std::string, std::string> output;
    output["message_type"] = message["message_type"].asString();
    output["is_request"] = message["is_request"].asString();
    output["message_id"] = message["payload"][0].asString();
    output["sender"] = message["payload"][1].asString();
    output["post_date"] = message["payload"][2].asString();
    output["subject"] = message["payload"][3].asString();
    output["content"] = message["payload"][4].asString();

    return output;
}

std::map<std::string, std::string> spam_api::parse::respond::leave(std::string& json_message) {
    Json::Reader reader;
    Json::Value message;
    reader.parse(json_message, message);

    std::map<std::string, std::string> output;
    output["message_type"] = message["message_type"].asString();
    output["is_request"] = message["is_request"].asString();
    output["success"] = message["success"].asString();

    return output;
}

std::map<std::string, std::variant<std::string, std::vector<std::string>>> spam_api::parse::respond::getusers(std::string& json_message) {
    Json::Reader reader;
    Json::Value message;
    reader.parse(json_message, message);

    std::map<std::string, std::variant<std::string, std::vector<std::string>>> output;
    output["message_type"] = message["message_type"].asString();
    output["is_request"] = message["is_request"].asString();
    std::vector<std::string> tempList;
    for(int i = 0; i < message["payload"].end() - message["payload"].begin(); i++) {
        tempList.push_back(message["payload"][i].asString());
    }
    output["users"] = tempList;

    return output;
}
