#pragma once

#include <common/types.h>

#include <WinSock2.h>

#include <string_view>

namespace common {

	class WsaContext {
	public:
		WsaContext();
		~WsaContext();

		static int GetLastWsaError();

		bool Init();

		// Called by the server.
		bool Listen(std::string_view port);

		// Called by the client.
		bool Connect(std::string_view hostname, std::string_view port);

		// Provides socket to perform send & receive operations on.
		const Socket& GetSocket() const;

	private:
		bool mInitialized{ false };
		WSAData mWsaData{};
		Socket mSocket{};
	};

}
