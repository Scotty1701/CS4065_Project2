#include "board_server.hpp"
#include "easy_winsock.hpp"
#include "spam_api.hpp"
#include <thread>
#include <iostream>

// Server Implementation
BoardServer::BoardServer(std::string port, int num_groups) {
    auto result = sockets.init_winsock(port);
    std::cout << "Started server on " << "127.0.0.1" << ":" << port << std::endl;
    sockets.create_listener();

    for (int i = 0; i < num_groups; i++) {
        groups.push_back(std::make_shared<Group>(i));
    }
}

// Cleanup sockets when the server closes
BoardServer::~BoardServer() {
    sockets.close_all_connections();
    sockets.shutdown_winsock();
}

void BoardServer::sendMessage(UserConnection& client, std::string message) {
    sockets.send_to_client(client.socket, message.c_str(), message.size());
}

// Handle receiving and responding to messages from clients
void interactWithClient(BoardServer* server, std::shared_ptr<UserConnection> client) {
    std::cout << "Started interaction function" << std::endl;
    std::cout << "Client: " << client->socket << std::endl;
    while (true) {
        // Get a message from the client
        auto result = server->sockets.receive_from_client(client->socket, client->buffer.get(), client->bufferLen);

        if (result == 0) {
            // Client disconnected, return
            break;
        }

        // Parse info from the message
        std::string newMessage{client->buffer.get()};
        std::cout << "read to buffer" << std::endl;
        parsedMessage fields = spam_api::parse(newMessage);
        std::cout << "parsed message" << std::endl;
        std::string messageType = std::get<std::string>(fields["message_type"]);
        std::cout << "Message type: " << messageType << std::endl;

        // Respond appropriately
        if (messageType == "connect") {
            std::string response = spam_api::gen::respond::connect(true, "Connected");
            server->sendMessage(*client, response);
            std::cout << "Sent response to connect" << std::endl;
        } else if (messageType == "join") {
            unsigned int group_id = (unsigned int)std::stoi(std::get<std::string>(fields["group_id"]));
            std::cout << client->name << " requests to join group " << group_id << std::endl;
            if (group_id >= server->groups.size()) {
                // Group out of range
                auto resp = spam_api::gen::respond::join(false, "Group id out of range");
                server->sendMessage(*client, resp);
                continue;
            }
            std::string newUsername = std::get<std::string>(fields["username"]);
            std::string response = spam_api::gen::respond::join(true, "User added");

            // Check for duplicate username
            for (auto client : server->groups.at(group_id)->clients) {
                if (newUsername == client->name) {
                    // Duplicate Username, send failed join response
                    std::cout << "User already in group!" << std::endl;
                    response = spam_api::gen::respond::join(false, "Duplicate username");
                    server->sendMessage(*client, response);

                }
            }
            if (std::get<std::string>(spam_api::parse(response)["success"]) == "false") {
                // Unable to add user, skip rest of this iteration and wait for next message
                continue;
            }

            // Didn't continue so username is new, add it to list and respond w/ success
            server->groups.at(group_id)->clients.push_back(std::make_shared<UserConnection>(*client));
            server->sendMessage(*client, response);
            client->name = std::get<std::string>(fields["username"]);

            // Let other users know a another user joined
            std::cout << "Yo new client just dropped" << std::endl;
            for (int i = 0; i < server->groups.at(group_id)->clients.size(); i++) {
                std::cout << "Notifying client: " << server->groups.at(group_id)->clients.at(i)->name << std::endl;
                std::string response = spam_api::gen::respond::getusers(server->groups.at(group_id)->getUsers());
                server->sendMessage(*server->groups.at(group_id)->clients.at(i), response);
            }
            // Inform client of last 2 messages posted
            std::cout << "sending previous messages" << std::endl;
            for (int i = server->groups.at(group_id)->messages.size();
                 (i > server->groups.at(group_id)->messages.size()-2) || (i > 0);
                 i--) {
                auto sender = server->groups.at(group_id)->messages.at(i-1)["sender"];
                auto post_date = server->groups.at(group_id)->messages.at(i-1)["post_date"];
                auto subject = server->groups.at(group_id)->messages.at(i-1)["subject"];
                auto content = server->groups.at(group_id)->messages.at(i-1)["content"];
                auto resp = spam_api::gen::respond::message(std::to_string(i-1), std::to_string(group_id), sender, post_date, subject, content);
                server->sendMessage(*client, resp);
            }
        } else if (messageType == "post") {
            std::cout << "Request for post" << std::endl;
            int group_id = std::stoi(std::get<std::string>(fields["group_id"]));
            // Store the message
            std::map<std::string, std::string> tempMessage;
            tempMessage["message_id"] = std::to_string(server->groups.at(group_id)->messages.size());
            tempMessage["sender"] = std::get<std::string>(fields["sender"]);
            tempMessage["post_date"] = std::get<std::string>(fields["post_date"]);
            tempMessage["subject"] = std::get<std::string>(fields["subject"]);
            tempMessage["content"] = std::get<std::string>(fields["content"]);
            server->groups.at(group_id)->messages.push_back(tempMessage);

            // Tell the client it was received successfully
            server->sendMessage(*client, spam_api::gen::respond::post(true, tempMessage["message_id"]));

            // Notify other users a new message is available
            for (int i = 0; i < server->groups.at(group_id)->clients.size(); i++) {
                std::cout << "New message available, notifying client: " << server->groups.at(group_id)->clients.at(i)->name << std::endl;
                std::string response = spam_api::gen::respond::message(tempMessage["message_id"], std::to_string(group_id), tempMessage["sender"], tempMessage["post_date"], tempMessage["subject"], tempMessage["content"]);
                server->sendMessage(*server->groups.at(group_id)->clients.at(i), response);
            }
        } else if (messageType == "message") {
            std::cout << "Request for message" << std::endl;
            unsigned int group_id = (unsigned int)std::stoi(std::get<std::string>(fields["group_id"]));
            if (group_id >= server->groups.size()) {
                // Group out of range
                auto resp = spam_api::gen::respond::message(false, "Group id out of range");
                server->sendMessage(*client, resp);
                continue;
            }
            auto id = std::stoi(std::get<std::string>(fields["message_id"]));
            if (id >= server->groups.at(group_id)->messages.size()) {
                // Requesting an invalid message id, send error message
                auto resp = spam_api::gen::respond::message(false, "invalid message id");
                server->sendMessage(*client, resp);
                continue;
            }
            auto sender = server->groups.at(group_id)->messages.at(id)["sender"];
            auto post_date = server->groups.at(group_id)->messages.at(id)["post_date"];
            auto subject = server->groups.at(group_id)->messages.at(id)["subject"];
            auto content = server->groups.at(group_id)->messages.at(id)["content"];
            auto resp = spam_api::gen::respond::message(std::to_string(id), std::to_string(group_id), sender, post_date, subject, content);
            server->sendMessage(*client, resp);
        } else if (messageType == "leave") {
            unsigned int group_id;
            try {
                group_id = (unsigned int)std::stoi(std::get<std::string>(fields["group_id"]));
            }
            catch(std::exception) {
                auto resp = spam_api::gen::respond::leave(false, "Invalid group id");
                server->sendMessage(*client, resp);
                continue;
            }
            if (group_id >= server->groups.size()) {
                // Group out of range
                auto resp = spam_api::gen::respond::message(false, "Group id out of range");
                server->sendMessage(*client, resp);
                continue;
            }
            std::cout << "User " << client->name << " is leaving group " << group_id << std::endl;
            auto resp = spam_api::gen::respond::leave(true, client->name);
            server->sendMessage(*client, resp);

            // Remove the client from the server's lists
            for (int i = 0; i < server->groups.at(group_id)->clients.size(); i++) {
                if (server->groups.at(group_id)->clients.at(i)->name == client->name) {
                    server->groups.at(group_id)->clients.erase(server->groups.at(group_id)->clients.begin()+i);
                    break;
                }
            }
            // Update the other users on the change
            for (int i = 0; i < server->groups.at(group_id)->clients.size(); i++) {
                std::cout << "Notifying client: " << server->groups.at(group_id)->clients.at(i)->name << std::endl;
                std::string response = spam_api::gen::respond::getusers(server->groups.at(group_id)->getUsers());
                server->sendMessage(*server->groups.at(group_id)->clients.at(i), response);
            }
        } else if (messageType == "getusers") {
            std::cout << "Request for getusers" << std::endl;
            int group_id = std::stoi(std::get<std::string>(fields["group_id"]));
            std::string response = spam_api::gen::respond::getusers(server->groups.at(group_id)->getUsers());
            server->sendMessage(*client, response);
        } else if (messageType == "getgroups") {
            std::cout << "Request for getgroups" << std::endl;
            std::vector<std::string> group_list;
            for (auto group : server->groups) {
                group_list.push_back(std::to_string(group->group_id));
            }
            std::string response = spam_api::gen::respond::getgroups(group_list);
            server->sendMessage(*client, response);
        }
    }

    // Erase client when thread is done
    std::cout << "Bye bye " << client->name << std::endl;
    //for (int i = 0; i < server->clients.size(); i++) {
    //    if (server->clients.at(i)->name == client->name) {
    //        server->clients.erase(server->clients.begin()+i);
    //        break;
    //    }
    //}
    return;
}

// Client Implementation
UserConnection::UserConnection(SOCKET socket, int bufferLen)
    : socket{socket}, bufferLen{bufferLen}, buffer{new char[bufferLen]}{};

// Group Implementation
Group::Group(int group_id)
    : group_id{group_id}{};

std::vector<std::string> Group::getUsers() {
    std::vector<std::string> names;
    for (auto client : clients) {
        names.push_back(client->name);
    }
    return names;
}

// Main event loop
int main() {
    std::vector<std::thread> threads;

    // Init the server
    BoardServer main_server("42000", 5);

    while (true) {
        // Listen for connections
        SOCKET client_socket{(SOCKET)main_server.sockets.accept_client()};
        auto new_client = std::make_shared<UserConnection>(client_socket, 1024);
        //main_server.clients.push_back(new_client);

        std::thread* tempThread = new std::thread(&interactWithClient, &main_server, new_client);
        tempThread->detach();
    }
    return 0;
}
