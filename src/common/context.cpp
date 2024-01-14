#include <common/context.h>

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

}
