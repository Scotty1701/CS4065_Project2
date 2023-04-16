#ifndef EASY_WINSOCK_HPP
#define EASY_WINSOCK_HPP

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <string>
#include <vector>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

class WinsockManager {
    private:
        SOCKET listenSocket;
        std::vector<SOCKET> clientSockets;
        WSADATA wsaData;
        struct addrinfo* result;
        struct addrinfo hints;

    public:
        WinsockManager();

        SOCKET get_client_socket(int index);

        int init_winsock(std::string port);

        int create_listener();

        int accept_client();

        int receive_from_client(SOCKET clientSocket, char buffer[], int bufferLen);

        void send_to_client(SOCKET clientSocket, const char buffer[], int bufferLen);

        int close_connection(SOCKET clientSocket);

        void close_all_connections();

        void shutdown_winsock();
};
#endif
