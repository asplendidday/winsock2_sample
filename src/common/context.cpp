#include <common/context.h>

#include <WS2tcpip.h>

#include <format>
#include <iostream>

namespace common {

	WsaContext::WsaContext() = default;

	WsaContext::~WsaContext() {
		if (mInitialized) {
			WSACleanup();
		}
	}

	int WsaContext::GetLastWsaError() {
		return WSAGetLastError();
	}

	bool WsaContext::Init() {
		// Silently ignore double init.
		if (mInitialized) {
			return true;
		}

		const int result = WSAStartup(MAKEWORD(2, 2), &mWsaData);
		mInitialized = (result == 0);
		return mInitialized;
	}

	bool WsaContext::Listen(std::string_view port) {
		// Fail if not initialized yet, or a connection has already been opened.
		if (!mInitialized || mSocket.IsValid()) {
			return false;
		}

		addrinfo hints{};
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		AddrInfo resultAddr{};

		int result = getaddrinfo(nullptr, port.data(), &hints, resultAddr.Put());
		if (result != 0) {
			std::cerr << std::format("[Listen] Failed to resolve local address (error code: {}).\n", GetLastWsaError());
			return false;
		}

		Socket listenSocket = socket(
			resultAddr->ai_family,
			resultAddr->ai_socktype,
			resultAddr->ai_protocol);
		if (!listenSocket.IsValid()) {
			std::cerr << std::format("[Listen] Failed to create listen socket (error code: {}).\n", GetLastWsaError());
			return false;
		}

		result = bind(
			listenSocket,
			resultAddr->ai_addr,
			static_cast<int>(resultAddr->ai_addrlen));
		if (result != 0) {
			std::cerr << std::format("[Listen] Failed to bind listen socket (error code: {}).\n", GetLastWsaError());
			return false;
		}

		result = listen(listenSocket, SOMAXCONN);
		if (result != 0) {
			std::cerr << std::format("[Listen] Failed to start listening (error code: {}).\n", GetLastWsaError());
			return false;
		}

		mSocket = accept(listenSocket, nullptr, nullptr);
		if (!mSocket.IsValid()) {
			std::cerr << std::format("[Listen] Failed to accept incoming connection (error code: {}).\n", GetLastWsaError());
			return false;
		}

		return true;
	}

	bool WsaContext::Connect(std::string_view hostname, std::string_view port) {
		// Fail if not initialized yet, or a connection has already been opened.
		if (!mInitialized || mSocket.IsValid()) {
			return false;
		}

		addrinfo hints{};
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		AddrInfo resultAddr{};

		int result = getaddrinfo(hostname.data(), port.data(), &hints, resultAddr.Put());
		if (result != 0) {
			std::cerr << std::format("[Connect] Failed to resolve host address (error code: {}).\n", GetLastWsaError());
			return false;
		}

		for (auto currAddr = resultAddr.Get(); currAddr != nullptr; currAddr = currAddr->ai_next) {
			mSocket = socket(
				currAddr->ai_family,
				currAddr->ai_socktype,
				currAddr->ai_protocol);
			if (!mSocket.IsValid()) {
				std::cerr << std::format("[Connect] Failed to create connection socket (error code: {}).\n", GetLastWsaError());
				return false;
			}

			result = connect(
				mSocket,
				currAddr->ai_addr,
				static_cast<int>(currAddr->ai_addrlen));
			if (result != 0) {
				mSocket.Reset();
				continue;
			}

			break;
		}

		if (!mSocket.IsValid()) {
			std::cerr << std::format("[Connect] Failed to establish a connection (error code: {}).\n", GetLastWsaError());
			return false;
		}

		return true;
	}

	const Socket& WsaContext::GetSocket() const {
		return mSocket;
	}

}
