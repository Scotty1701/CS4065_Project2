#include "board_server.hpp"
#include "easy_winsock.hpp"
#include "spam_api.hpp"
#include <thread>
#include <iostream>

// Server Implementation
BoardServer::BoardServer(std::string port) {
    sockets.init_winsock(port);
    sockets.create_listener();
}

void BoardServer::sendMessage(UserConnection& client, std::string message) {
    sockets.send_to_client(client.socket, message.c_str(), message.size());
}

//TODO: Convert to method of BoardServer (if possible)?
void interactWithClient(BoardServer* server, UserConnection* client) {
    std::cout << "Started interaction function" << std::endl;
    std::cout << "Client: " << client->socket << std::endl;
    while (true) {
        // Get a message from the client
        server->sockets.receive_from_client(client->socket, client->buffer.get(), client->bufferLen);
        std::cout << "receiving from client" << std::endl;
        std::string newMessage{client->buffer.get()};
        parsedMessage fields = spam_api::parse(newMessage);
        std::string messageType = std::get<std::string>(fields["message_type"]);
        std::cout << "Message type: " << messageType << std::endl;

        // Respond appropriately
        if (messageType == "connect") {
            // Don't need?
        } else if (messageType == "join") {
            std::cout << "Request for join" << std::endl;
            std::string newUsername = std::get<std::string>(fields["username"]);
            for (std::string username : server->clientUsernames) {
                if (newUsername == username) {
                    // Duplicate Username
                    // Send failed join response
                    std::string response = spam_api::gen::respond::join(false, "Duplicate username");
                    server->sendMessage(*client, response);
                    // TODO: Cleanup this client and close the port
                    // TODO: Return from this thread
                    std::cout << "Returning" << std::endl;
                    return;
                }
            }

            // Didn't return so username is new, add it to list and respond w/ success
            server->clientUsernames.push_back(std::get<std::string>(fields["username"]));
            std::string response = spam_api::gen::respond::join(true, "User added");
            server->sendMessage(*client, response);

            // TODO: Let other users know a another user joined
            // TODO: Inform client of last 2 messages posted
            std::cout << "sending previous messages" << std::endl;
            std::cout << server->messages.size() << std::endl;
            for (int i = server->messages.size();
                 (i > server->messages.size()-2) || (i > 0);
                 i--) {
                auto sender = server->messages.at(i-1)["sender"];
                auto post_date = server->messages.at(i-1)["post_date"];
                auto subject = server->messages.at(i-1)["subject"];
                auto content = server->messages.at(i-1)["content"];
                auto resp = spam_api::gen::respond::message(std::to_string(i-1), sender, post_date, subject, content);
                server->sendMessage(*client, resp);
                std::cout << std::to_string(i) << std::endl;
                std::cout << subject << ", " << content << std::endl;
            }
        } else if (messageType == "post") {
            std::cout << "Request for post" << std::endl;
            // Store the message
            std::map<std::string, std::string> tempMessage;
            //tempMessage["message_id"] = std::get<std::string>(fields["message_id"]);
            tempMessage["message_id"] = std::to_string(server->messages.size());
            tempMessage["sender"] = std::get<std::string>(fields["sender"]);
            tempMessage["post_date"] = std::get<std::string>(fields["post_date"]);
            tempMessage["subject"] = std::get<std::string>(fields["subject"]);
            tempMessage["content"] = std::get<std::string>(fields["content"]);
            server->messages.push_back(tempMessage);

            // Tell the client it was received successfully
            server->sendMessage(*client, spam_api::gen::respond::post(true, tempMessage["message_id"]));

            // TODO: Notify other users a new message is available
        } else if (messageType == "message") {
            std::cout << "Request for message" << std::endl;
            auto id = std::stoi(std::get<std::string>(fields["message_id"]));
            if (id >= server->messages.size()) {
                // Requesting an invalid message id
                // Send error message
                auto resp = spam_api::gen::respond::message(false, "invalid message id");
                server->sendMessage(*client, resp);
                continue;
            }
            auto sender = server->messages.at(id)["sender"];
            auto post_date = server->messages.at(id)["post_date"];
            auto subject = server->messages.at(id)["subject"];
            auto content = server->messages.at(id)["content"];
            auto resp = spam_api::gen::respond::message(std::to_string(id), sender, post_date, subject, content);
            server->sendMessage(*client, resp);
        } else if (messageType == "leave") {
            // TODO
        } else if (messageType == "getusers") {
            std::cout << "Request for getusers" << std::endl;
            std::string response = spam_api::gen::respond::getusers(server->clientUsernames);
            server->sendMessage(*client, response);
        }
    }
}

// Client Implementation
UserConnection::UserConnection(SOCKET socket, int bufferLen)
    : socket{socket}, bufferLen{bufferLen}, buffer{new char[bufferLen]}{};


// Main event loop
int main() {
    std::vector<std::thread> threads;

    // Init the server
    BoardServer main_server("42000");

    while (true) {
        // Listen for connections
        SOCKET client_socket{(SOCKET)main_server.sockets.accept_client()};
        auto new_client = std::make_shared<UserConnection>(client_socket, 1024);
        main_server.clients.push_back(new_client);

        std::thread* tempThread = new std::thread(&interactWithClient, &main_server, new_client.get());
        tempThread->detach();
    }
    return 0;
}
