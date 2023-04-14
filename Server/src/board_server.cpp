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
        } else if (messageType == "join") {
            std::cout << "Request for join" << std::endl;
            std::string newUsername = std::get<std::string>(fields["username"]);
            for (std::string username : server->clientUsernames) {
                if (newUsername == username) {
                    // Duplicate Username
                    // Send failed join response
                    std::string response = spam_api::gen::respond::join(false);
                    server->sendMessage(*client, response);
                    // TODO: Cleanup this client and close the port
                    // TODO: Return from this thread
                    std::cout << "Returning" << std::endl;
                    return;
                }
            }

            // Didn't return so username is new, add it to list and respond w/ success
            server->clientUsernames.push_back(std::get<std::string>(fields["username"]));
            std::string response = spam_api::gen::respond::join(true);
            server->sendMessage(*client, response);
        } else if (messageType == "post") {
            // TODO
        } else if (messageType == "message") {
            // TODO
        } else if (messageType == "leave") {
            // TODO
        } else if (messageType == "getusers") {
            std::cout << "Request for getusers" << std::endl;
            std::string response = spam_api::gen::respond::getusers(server->clientUsernames);
            server->sendMessage(*client, response);
            return;
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
