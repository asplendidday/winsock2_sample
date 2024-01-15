#include <common/types.h>

#include <WS2tcpip.h>

#include <algorithm>

namespace common {

	Socket::~Socket() {
		Reset();
	}
	
	Socket::Socket(SOCKET socket)
		: mSocket{ socket }
	{ }

	Socket& Socket::operator=(SOCKET socket) {
		Reset();
		mSocket = socket;
		return *this;
	}

	Socket::Socket(Socket&& other) noexcept {
		*this = std::move(other);
	}

	Socket& Socket::operator=(Socket&& other) noexcept {
		std::swap(mSocket, other.mSocket);
		return *this;
	}

	bool Socket::IsValid() const {
		return (mSocket != INVALID_SOCKET);
	}

	void Socket::Reset() {
		if (IsValid()) {
			closesocket(mSocket);
			mSocket = INVALID_SOCKET;
		}
	}

	AddrInfo::~AddrInfo() {
		Reset();
	}

	AddrInfo::AddrInfo(addrinfo* addrInfo)
		: mAddrInfo{ addrInfo }
	{ }

	AddrInfo& AddrInfo::operator=(addrinfo* addrInfo) {
		Reset();
		mAddrInfo = addrInfo;
		return *this;
	}

	void AddrInfo::Reset() {
		if (mAddrInfo != nullptr) {
			freeaddrinfo(mAddrInfo);
			mAddrInfo = nullptr;
		}
	}
}
