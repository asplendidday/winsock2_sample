#pragma once

#include <WinSock2.h>

namespace common {

	class Socket final {
	public:
		~Socket();

		Socket(SOCKET socket = INVALID_SOCKET);
		Socket& operator=(SOCKET socket);

		Socket(const Socket&) = delete;
		Socket& operator=(const Socket&) = delete;

		Socket(Socket&& other) noexcept;
		Socket& operator=(Socket&& other) noexcept;

		inline operator SOCKET() const { return mSocket; }

		bool IsValid() const;

		void Reset();

	private:
		SOCKET mSocket{ INVALID_SOCKET };
	};

	class AddrInfo final {
	public:
		~AddrInfo();

		AddrInfo(addrinfo* addrInfo = nullptr);
		AddrInfo& operator=(addrinfo* addrinfo);

		AddrInfo(const AddrInfo&) = delete;
		AddrInfo& operator=(const AddrInfo&) = delete;

		AddrInfo(AddrInfo&&) = delete;
		AddrInfo& operator=(AddrInfo&&) = delete;

		inline addrinfo* operator->() { return mAddrInfo; }

		inline addrinfo** Put() { return &mAddrInfo; }

		inline addrinfo* Get() { return mAddrInfo; }

		void Reset();

	private:
		struct addrinfo* mAddrInfo{ nullptr };
	};

}
