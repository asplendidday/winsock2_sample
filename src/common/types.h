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

}
