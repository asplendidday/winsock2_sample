#include <common/context.h>
#include <common/types.h>

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <format>
#include <iostream>

int main(int argc, char** argv) {
	common::WsaContext wsaContext{};
	if (!wsaContext.Init()) {
		std::cerr << "[Server] Failed to initialize WSA context.\n";
		return -1;
	}

	if (!wsaContext.Listen("9000")) {
		std::cerr << "[Server] Failed to establish a connection.\n";
		return -1;
	}

	char recvBuffer[1500];

	int result = 0;

	do {
		result = recv(wsaContext.GetSocket(), recvBuffer, sizeof(recvBuffer), 0);

		if (result > 0) {
			std::cout << std::format("[Server] Received {} bytes.\n", result);
			std::cout << "  Message: " << std::string{ recvBuffer, static_cast<size_t>(result) } << "\n";

			// Echo message back.
			result = send(wsaContext.GetSocket(), recvBuffer, result, 0);
			if (result < 0) {
				std::cerr << "[Server] Failed to send answer message.\n";
				return -1;
			}
			else if (result > 0) {
				std::cout << std::format("[Server] Sent {} bytes.\n", result);
			}
		}
		else if (result == 0) {
			std::cout << "[Server] Connection closing.\n";
		}
		else {
			std::cerr << std::format("[Server] A receive error occurred: {}\n", WSAGetLastError());
			return -1;
		}
	} while (result > 0);

	result = shutdown(wsaContext.GetSocket(), SD_SEND);		
	if (result != 0) {
		std::cerr << "[Server] An error occurred during shutdown.\n";
		return -1;
	}

	return 0;
}
