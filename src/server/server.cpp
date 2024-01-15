#include <common/context.h>
#include <common/types.h>

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <iostream>

int main(int argc, char** argv) {
	common::WsaContext wsaContext{};
	if (!wsaContext.Init()) {
		std::cerr << "Failed to initializer WSA.\n";
		return -1;
	}

	addrinfo hints{};
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	common::AddrInfo resultAddr{};

	int result = getaddrinfo(nullptr, "9000", &hints, resultAddr.Put());
	if (result != 0) {
		std::cerr << "Failed to resolve address.\n";
		return -1;
	}

	common::Socket listenSocket = socket(
		resultAddr->ai_family,
		resultAddr->ai_socktype,
		resultAddr->ai_protocol);
	if (!listenSocket.IsValid()) {
		std::cerr << "Failed to create listen socket.\n";
		return -1;
	}

	result = bind(
		listenSocket,
		resultAddr->ai_addr,
		static_cast<int>(resultAddr->ai_addrlen));
	if (result != 0) {
		std::cerr << "Failed to bind listen socket.\n";
		return -1;
	}

	resultAddr.Reset();

	result = listen(listenSocket, SOMAXCONN);
	if (result != 0) {
		std::cerr << "Failed to start listening.\n";
		return -1;
	}

	common::Socket connectionSocket = accept(listenSocket, nullptr, nullptr);
	if (connectionSocket == INVALID_SOCKET) {
		const int lastError = WSAGetLastError();
		std::cerr << "Failed to accept connection (" << lastError << ").\n";
		return -1;
	}

	listenSocket.Reset();

	char recvBuffer[1500];

	do {
		result = recv(connectionSocket, recvBuffer, sizeof(recvBuffer), 0);

		if (result > 0) {
			std::cout << "Received " << result << " bytes.\n";
			std::cout << "  Message: " << std::string{ recvBuffer, static_cast<size_t>(result) } << "\n";

			// Echo message back.
			result = send(connectionSocket, recvBuffer, result, 0);
			if (result < 0) {
				std::cerr << "Failed to send answer message.\n";
				return -1;
			}
			else if (result > 0) {
				std::cout << "Sent " << result << " bytes.\n";
			}
		}
		else if (result == 0) {
			std::cout << "Connection closing.\n";
		}
		else {
			std::cerr << "A receive error occurred: " << WSAGetLastError() << "\n";
			return -1;
		}
	} while (result > 0);

	result = shutdown(connectionSocket, SD_SEND);		
	if (result != 0) {
		std::cerr << "An error occurred during shutdown.\n";
		return -1;
	}

	return 0;
}
