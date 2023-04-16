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

std::string spam_api::gen::respond::connect(bool success, std::string message) {
    Json::Value response;

    response["message_type"] = "connect";
    response["is_request"] = false;
    response["success"] = success;
    response["payload"] = message;

    std::string output = response.toStyledString();

    return output;
}

std::string spam_api::gen::request::join(std::string username, std::string group_id) {
    Json::Value message;

    message["message_type"] = "join";
    message["is_request"] = true;
    message["payload"].append(username);
    message["payload"].append(group_id);

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen::respond::join(bool success, std::string message) {
    Json::Value response;

    response["message_type"] = "join";
    response["is_request"] = false;
    response["success"] = success;
    response["payload"] = message;

    std::string output = response.toStyledString();

    return output;
}

std::string spam_api::gen::request::post(std::string group_id, std::string sender, std::string post_date, std::string subject, std::string content) {
    Json::Value message;

    message["message_type"] = "post";
    message["is_request"] = true;
    message["payload"].append(group_id);
    message["payload"].append(sender);
    message["payload"].append(post_date);
    message["payload"].append(subject);
    message["payload"].append(content);

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen::respond::post(bool success, std::string message) {
    Json::Value response;

    response["message_type"] = "post";
    response["is_request"] = false;
    response["success"] = success;
    response["payload"] = message; // Message ID if successful, error message otherwise

    std::string output = response.toStyledString();

    return output;
}

std::string spam_api::gen::request::message(std::string group_id, std::string message_id) {
    Json::Value message;

    message["message_type"] = "message";
    message["is_request"] = true;
    message["payload"].append(group_id);
    message["payload"].append(message_id);

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen::respond::message(std::string message_id, std::string group_id, std::string sender, std::string post_date, std::string subject, std::string content) {
    Json::Value message;

    message["message_type"] = "message";
    message["is_request"] = false;
    message["success"] = true;
    message["payload"].append(message_id);
    message["payload"].append(group_id);
    message["payload"].append(sender);
    message["payload"].append(post_date);
    message["payload"].append(subject);
    message["payload"].append(content);

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen::respond::message(bool success, std::string message) {
    Json::Value response;

    response["message_type"] = "message";
    response["is_request"] = false;
    response["success"] = success;
    response["payload"] = message;

    std::string output = response.toStyledString();

    return output;
}

std::string spam_api::gen::request::leave(std::string group_id) {
    Json::Value message;

    message["message_type"] = "leave";
    message["is_request"] = true;
    message["payload"] = group_id;

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen::respond::leave(bool success, std::string message) {
    Json::Value response;

    response["message_type"] = "leave";
    response["is_request"] = false;
    response["success"] = success;
    response["payload"] = message;

    std::string output = response.toStyledString();

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
    message["success"] = true;
    for (std::string user : users) {
        message["payload"].append(user);
    }

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen::respond::getusers(bool success, std::string message) {
    Json::Value response;

    response["message_type"] = "getusers";
    response["is_request"] = false;
    response["success"] = success;
    response["payload"] = message;

    std::string output = response.toStyledString();

    return output;
}

std::string spam_api::gen::request::getgroups() {
    Json::Value request;

    request["message_type"] = "getgroups";
    request["is_request"] = true;

    std::string output = request.toStyledString();

    return output;
}

std::string spam_api::gen::respond::getgroups(std::vector<std::string> groups) {
    Json::Value message;

    message["message_type"] = "getgroups";
    message["is_request"] = false;
    message["success"] = true;
    for (std::string group : groups) {
        message["payload"].append(group);
    }

    std::string output = message.toStyledString();

    return output;
}

std::string spam_api::gen::respond::getgroups(bool success, std::string message) {
    Json::Value response;

    response["message_type"] = "getgroups";
    response["is_request"] = false;
    response["success"] = success;
    response["payload"] = message;

    std::string output = response.toStyledString();

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
            output["sender"] = message["payload"][0].asString();
            output["post_date"] = message["payload"][1].asString();
            output["subject"] = message["payload"][2].asString();
            output["content"] = message["payload"][3].asString();
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
            output["success"] = message["success"].asString();
            output["payload"] = message["payload"].asString();
        } else if (message_type == "join") {
            output["success"] = message["success"].asString();
            output["payload"] = message["payload"].asString();
        } else if (message_type == "post") {
            output["success"] = message["success"].asString();
            output["payload"] = message["payload"].asString();
        } else if (message_type == "message") {
            if (message["success"].asString() == "true") {
                output["message_id"] = message["payload"][0].asString();
                output["sender"] = message["payload"][1].asString();
                output["post_date"] = message["payload"][2].asString();
                output["subject"] = message["payload"][3].asString();
                output["content"] = message["payload"][4].asString();
            }
            else {
                output["success"] = message["success"].asString();
                output["payload"] = message["payload"].asString();
            }
        } else if (message_type == "leave") {
            output["success"] = message["success"].asString();
            output["payload"] = message["payload"].asString();
        } else if (message_type == "getusers") {
            if (message["success"].asString() == "true") {
                std::vector<std::string> tempList;
                for(int i = 0; i < message["payload"].end() - message["payload"].begin(); i++) {
                    tempList.push_back(message["payload"][i].asString());
                }
                output["users"] = tempList;
            }
            else {
                output["success"] = message["success"].asString();
                output["payload"] = message["payload"].asString();
            }
        }
    }

    return output;
}
