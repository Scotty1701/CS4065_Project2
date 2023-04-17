#ifndef EASY_WINSOCK_HPP
#define EASY_WINSOCK_HPP

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#ifdef _WIN32
#include <windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <exception>
typedef std::exception exception;
#pragma comment (lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <errno.h>
#include <stdexcept>
#define ZeroMemory(Destination, Length) memset((Destination),0,(Length))

typedef int SOCKET;
typedef std::runtime_error exception;
const auto closesocket = close;
const auto WSACleanup = []{return 0;};
const auto WSAGetLastError = []{return errno;};
typedef std::runtime_error exception;
const int SOCKET_ERROR = -1;
const int INVALID_SOCKET = -1;
const int SD_SEND = 1;

#endif
#include <stdlib.h>
#include <string>
#include <vector>

// Need to link with Ws2_32.lib

class WinsockManager {
    private:
        SOCKET listenSocket;
        std::vector<SOCKET> clientSockets;
        #ifdef _WIN32
          WSADATA wsaData;
        #endif
        struct addrinfo* result;
        struct addrinfo hints;

    public:
        WinsockManager();

        SOCKET get_client_socket(int index);

        std::string init_winsock(std::string port);

        int create_listener();

        int accept_client();

        int receive_from_client(SOCKET clientSocket, char buffer[], int bufferLen);

        void send_to_client(SOCKET clientSocket, const char buffer[], int bufferLen);

        int close_connection(SOCKET clientSocket);

        void close_all_connections();

        void shutdown_winsock();
};

#endif
