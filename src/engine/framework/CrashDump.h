#pragma once

#include <common/String.h>
#include <common/Util.h>

namespace Sys {
    // Ensure existence of the subdirectory of homepath used for crash dumps. Return true if successful
    bool CreateCrashDumpPath();

    // Launch the Breakpad server, if enabled in CMake and cvars.
    void BreakpadInit();

    // Write a crash dump that came from an NaCl VM.
    void NaclCrashDump(const std::vector<uint8_t>& dump, Str::StringRef vmName);
}