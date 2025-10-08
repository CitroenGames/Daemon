#pragma once 

#include "common/Common.h"
#include "common/IPC/Channel.h"

namespace VM {

    class VMBase;

    class CommonVMServices {
        public:
            CommonVMServices(VMBase& vm, Str::StringRef vmName, FS::Owner fileOwnership, int commandFlag);
            ~CommonVMServices();

            void Syscall(int major, int minor, Util::Reader reader, IPC::Channel& channel);

        private:
            std::string vmName;
            FS::Owner fileOwnership;
            VMBase& vm;

            VMBase& GetVM();

            // Misc Related
            void HandleMiscSyscall(int minor, Util::Reader& reader, IPC::Channel& channel);

            // Command Related
            void HandleCommandSyscall(int minor, Util::Reader& reader, IPC::Channel& channel);

            void AddCommand(Util::Reader& reader, IPC::Channel& channel);
            void RemoveCommand(Util::Reader& reader, IPC::Channel& channel);
            void EnvPrint(Util::Reader& reader, IPC::Channel& channel);
            void EnvExecuteAfter(Util::Reader& reader, IPC::Channel& channel);

            class ProxyCmd;
            std::unique_ptr<ProxyCmd> commandProxy;
            //TODO use the values to help the VM cache the location of the commands instead of doing a second hashtable lookup
            std::unordered_map<std::string, uint64_t> registeredCommands;

            // Cvar Related
            void HandleCvarSyscall(int minor, Util::Reader& reader, IPC::Channel& channel);

            void RegisterCvar(Util::Reader& reader, IPC::Channel& channel);
            void GetCvar(Util::Reader& reader, IPC::Channel& channel);
            void SetCvar(Util::Reader& reader, IPC::Channel& channel);
            void AddCvarFlags(Util::Reader& reader, IPC::Channel& channel);

            class ProxyCvar;
            std::vector<std::unique_ptr<ProxyCvar>> registeredCvars;

            // Log Related
            void HandleLogSyscall(int minor, Util::Reader& reader, IPC::Channel& channel);

            // Common common QVM syscalls
            void HandleCommonQVMSyscall(int minor, Util::Reader& reader, IPC::Channel& channel);
    };
}