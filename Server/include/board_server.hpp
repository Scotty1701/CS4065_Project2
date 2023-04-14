#include "easy_winsock.hpp"
#include "spam_api.hpp"

#include <variant>
#include <vector>
#include <memory>
#include <map>

class UserConnection {
    public:
        SOCKET socket;
        std::string name;

        int bufferLen;
        std::unique_ptr<char[]> buffer;

        UserConnection(SOCKET socket, int bufferLen);
};

class BoardServer {
    private:

    public:
        WinsockManager sockets;
        std::vector<std::shared_ptr<UserConnection>> clients;
        std::vector<std::string> clientUsernames;
        std::vector<std::map<std::string, std::string>> messages;

        BoardServer(std::string port);

        void sendMessage(UserConnection& client, std::string message);

        //TODO: is below workable?
        //void interactWithClient(UserConnection client);
};
