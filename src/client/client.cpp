#include <common/context.h>
#include <common/types.h>

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <iostream>

#pragma comment (lib, "Ws2_32.lib")

int main(int argc, char** argv) {
	common::WsaContext wsaContext{};
	if (!wsaContext.Init()) {
		std::cerr << "Failed to initialize WSA context.\n";
		return -1;
	}

	addrinfo hints{};
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	common::AddrInfo resultAddr;

	int result = getaddrinfo("127.0.0.1", "9000", &hints, resultAddr.Put());
	if (result != 0) {
		std::cerr << "Failed to resolve address.\n";
		return -1;
	}

	common::Socket connectionSocket{};

	for (auto currAddr = resultAddr.Get(); currAddr != nullptr; currAddr = currAddr->ai_next) {
		connectionSocket = socket(
			currAddr->ai_family,
			currAddr->ai_socktype,
			currAddr->ai_protocol);
		if (!connectionSocket.IsValid()) {
			std::cerr << "Failed to create connection socket.\n";
			return -1;
		}

		result = connect(
			connectionSocket,
			currAddr->ai_addr,
			static_cast<int>(currAddr->ai_addrlen));
		if (result != 0) {
			connectionSocket.Reset();
			continue;
		}
		
		break;
	}

	resultAddr.Reset();

	if (!connectionSocket.IsValid()) {
		std::cerr << "Failed to open a connection socket.\n";
		return -1;
	}

	const char* message = "Hello, endpoint!";

	result = send(connectionSocket, message, static_cast<int>(strlen(message)), 0);
	if (result < 0) {
		std::cerr << "Failed to send message.\n";
		return -1;
	}

	std::cout << "Sent " << result << " bytes.\n";

	result = shutdown(connectionSocket, SD_SEND);
	if (result != 0) {
		std::cerr << "An error occurred during shutdown.\n";
		return -1;
	}

	char recvBuffer[1500];

	do
	{
		result = recv(connectionSocket, recvBuffer, sizeof(recvBuffer), 0);
		if (result > 0) {
			std::cout << "Received " << result << " bytes.\n";
			std::cout << "  Message: " << std::string{ recvBuffer, static_cast<size_t>(result) } << "\n";
		}
		else if (result == 0) {
			std::cout << "Connection closing.\n";
		}
		else {
			std::cerr << "A receive error occurred: " << WSAGetLastError() << "\n";
			return -1;
		}
	} while (result > 0);

	return 0;
}