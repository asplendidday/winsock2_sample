#pragma once

#include <WinSock2.h>

namespace common {

	class WsaContext {
	public:
		WsaContext();
		~WsaContext();

		static int GetLastWsaError();

		bool Init();

	private:
		bool mInitialized{ false };
		WSAData mWsaData{};
	};

}
