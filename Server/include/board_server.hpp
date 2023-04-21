#include "easy_winsock.hpp"
#include "spam_api.hpp"

#include <variant>
#include <vector>
#include <memory>
#include <map>

// Simple class to hold important bits of information for each
// client connection
class UserConnection {
    public:
        SOCKET socket;
        std::string name;

        int bufferLen;
        std::shared_ptr<char[]> buffer;

        UserConnection(SOCKET socket, int bufferLen);
};

// Simple class that defines a group in the message board
// Tracks the users and messages within a group
class Group {
    public:
        int group_id;
        //std::vector<std::string> clientUsernames;
        std::vector<std::shared_ptr<UserConnection>> clients;
        std::vector<std::map<std::string, std::string>> messages;

        std::vector<std::string> getUsers();

        Group(int group_id);
};

// Main class to hold information for the message board
class BoardServer {
    private:

    public:
        WinsockManager sockets;
        //std::vector<std::shared_ptr<UserConnection>> clients;
        std::vector<std::shared_ptr<Group>> groups;

        // Start the server on the given port with the given number of groups
        BoardServer(std::string port, int num_groups);
        ~BoardServer();

        // Send a message to the specified client
        void sendMessage(UserConnection& client, std::string message);
};
