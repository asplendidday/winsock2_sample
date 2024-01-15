#include <common/context.h>
#include <common/types.h>

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <iostream>

int main(int argc, char** argv) {
	common::WsaContext wsaContext{};
	if (!wsaContext.Init()) {
		std::cerr << "Failed to initialize WSA context.\n";
		return -1;
	}

	if (!wsaContext.Connect("127.0.0.1", "9000")) {
		std::cerr << "[Client] Failed to establish a connection.\n";
		return -1;
	}

	const char* message = "Hello, endpoint!";

	int result = send(wsaContext.GetSocket(), message, static_cast<int>(strlen(message)), 0);
	if (result < 0) {
		std::cerr << "Failed to send message.\n";
		return -1;
	}

	std::cout << "Sent " << result << " bytes.\n";

	result = shutdown(wsaContext.GetSocket(), SD_SEND);
	if (result != 0) {
		std::cerr << "An error occurred during shutdown.\n";
		return -1;
	}

	char recvBuffer[1500];

	do
	{
		result = recv(wsaContext.GetSocket(), recvBuffer, sizeof(recvBuffer), 0);
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