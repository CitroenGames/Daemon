#pragma once

#include "common/IPC/Channel.h"

namespace VM {

	// Root channel used to communicate with the engine
	extern IPC::Channel rootChannel;

#ifdef BUILD_VM_NATIVE_EXE
	extern Cvar::Cvar<bool> useNativeExeCrashHandler;
#endif

	// Functions each specific gamelogic should implement
	void VMInit();
	void VMHandleSyscall(uint32_t id, Util::Reader reader);
	void GetNetcodeTables(NetcodeTable& playerStateTable, int& playerStateSize);

	// Send a message to the engine
	template<typename Msg, typename... Args> void SendMsg(Args&&... args) {
		if (!Sys::OnMainThread()) {
			Sys::Error("SendMsg from non-main VM thread");
		}
		IPC::SendMsg<Msg>(rootChannel, VMHandleSyscall, std::forward<Args>(args)...);
	}

}