#pragma once

#include "qcommon/qcommon.h"

// Low-level system functions
namespace Sys {

// Cleanly exit the engine, shutting down all subsystems.
NORETURN void Quit(Str::StringRef message);

// Get the path of a singleton socket
std::string GetSingletonSocketPath();

} // namespace Sys