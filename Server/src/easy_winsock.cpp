#include "easy_winsock.hpp"


WinsockManager::WinsockManager() {
    listenSocket = INVALID_SOCKET;
    result = NULL;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
}

SOCKET WinsockManager::get_client_socket(int index) {
    return clientSockets.at(index);
}
int WinsockManager::init_winsock(std::string port) {
    #if _WIN32
      int startup_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    #else
      int startup_result = 0;
    #endif

    if (startup_result != 0) {
        std::string message = "WSAStartup failed with error: " + std::to_string(startup_result);
        typedef std::runtime_error exception;
        throw exception(message.c_str());
    }

    int get_addr_info_result = getaddrinfo(NULL, port.c_str(), &hints, &result);

    if (get_addr_info_result != 0) {
        std::string message = "getaddrinfo failed with error: " + std::to_string(get_addr_info_result);
        throw exception(message.c_str());
    }

    return get_addr_info_result;
}

int WinsockManager::create_listener() {
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (listenSocket == INVALID_SOCKET) {
        std::string message = "Socket failed with error: " + std::to_string(WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        throw exception(message.c_str());
    }

    int bind_result = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);

    if (bind_result == SOCKET_ERROR) {
        std::string message = "Bind failed with error: " + std::to_string(WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        throw exception(message.c_str());
    }

    int listen_result = listen(listenSocket, SOMAXCONN);

    if (listen_result == SOCKET_ERROR) {
        std::string message = "Listen failed with error: " + std::to_string(WSAGetLastError());
        throw exception(message.c_str());
    }

    return listen_result;
}

int WinsockManager::accept_client() {
    int tempSocket = accept(listenSocket, NULL, NULL);

    if (tempSocket == INVALID_SOCKET) {
        std::string message = "Accept failed with error: " + std::to_string(WSAGetLastError());
        throw exception(message.c_str());
    }

    clientSockets.push_back(tempSocket);
    return clientSockets.back();
}

int WinsockManager::receive_from_client(SOCKET clientSocket, char buffer[], int bufferLen) {
    int receive_result = recv(clientSocket, buffer, bufferLen, 0);

    if (receive_result < 0) {
        std::string message = "Recv failed with error: " + std::to_string(WSAGetLastError());
        throw exception(message.c_str());
    }

    return receive_result;
}

void WinsockManager::send_to_client(SOCKET clientSocket, const char buffer[], int bufferLen) {
    int send_result = send(clientSocket, buffer, bufferLen, 0);

    if (send_result < 0) {
        std::string message = "Send failed with error: " + std::to_string(WSAGetLastError());
        throw exception(message.c_str());
    }
}

int WinsockManager::close_connection(SOCKET clientSocket) {
    int close_result = shutdown(clientSocket, SD_SEND);

    if (close_result == SOCKET_ERROR) {
        std::string message = "Shutdown failed with error: " + std::to_string(WSAGetLastError());
        throw exception(message.c_str());
    }
    closesocket(clientSocket);

    return close_result;
}

void WinsockManager::close_all_connections() {
    for (SOCKET client : clientSockets) {
        WinsockManager::close_connection(client);
    }
}

void WinsockManager::shutdown_winsock() {
  WSACleanup();
}
