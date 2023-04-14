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

parsedMessage spam_api::parse(std::string& json_message) {
    Json::Reader reader;
    Json::Value message;
    reader.parse(json_message, message);

    parsedMessage output;
    output["message_type"] = message["message_type"].asString();
    output["is_request"] = message["is_request"].asString();

    // Extract the message payload based on its type
    std::string message_type = std::get<std::string>(output["message_type"]);
    // Parse requests
    if (std::get<std::string>(output["is_request"]) == "true") {
        if (message_type == "connect") {
            output["address"] = message["payload"][0].asString();
            output["port"] = message["payload"][1].asString();
        } else if (message_type == "join") {
            output["username"] = message["payload"].asString();
        } else if (message_type == "post") {
            output["message_id"] = message["payload"][0].asString();
            output["sender"] = message["payload"][1].asString();
            output["post_date"] = message["payload"][2].asString();
            output["subject"] = message["payload"][3].asString();
            output["content"] = message["payload"][4].asString();
        } else if (message_type == "message") {
            output["message_id"] = message["payload"].asString();
        } else if (message_type == "leave") {
            output["username"] = message["payload"].asString();
        } else if (message_type == "getusers") {
            output["group_id"] = message["payload"].asString();
        }
    // Parse responses
    } else {
        if (message_type == "connect") {
            output["success"] = message["payload"].asString();
        } else if (message_type == "join") {
            output["success"] = message["payload"].asString();
        } else if (message_type == "post") {
            output["success"] = message["payload"].asString();
        } else if (message_type == "message") {
            output["message_id"] = message["payload"][0].asString();
            output["sender"] = message["payload"][1].asString();
            output["post_date"] = message["payload"][2].asString();
            output["subject"] = message["payload"][3].asString();
            output["content"] = message["payload"][4].asString();
        } else if (message_type == "leave") {
            output["success"] = message["payload"].asString();
        } else if (message_type == "getusers") {
            std::vector<std::string> tempList;
            for(int i = 0; i < message["payload"].end() - message["payload"].begin(); i++) {
                tempList.push_back(message["payload"][i].asString());
            }
            output["users"] = tempList;
        }
    }

    return output;
}
