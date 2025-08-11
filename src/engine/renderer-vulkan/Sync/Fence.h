/*
===========================================================================

Daemon BSD Source Code
Copyright (c) 2025 Daemon Developers
All rights reserved.

This file is part of the Daemon BSD Source Code (Daemon Source Code).

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the Daemon developers nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL DAEMON DEVELOPERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

===========================================================================
*/
// Fence.h

#ifndef FENCE_H
#define FENCE_H

#include <atomic>

#include "../Math/NumberTypes.h"

struct Fence;

struct FenceMain {
	std::atomic<uint64> value;
	std::atomic<bool> done = false;
	uint64 target = 0;

	void Signal();

	void Wait( const std::memory_order order = std::memory_order_relaxed );

	FenceMain() = default;
	FenceMain( const FenceMain& other ) = delete;
	FenceMain( FenceMain&& other ) = delete;

	Fence Target( const uint64 target );

	void operator=( const FenceMain& other ) = delete;
};

struct Fence {
	std::atomic<uint64>* value;
	std::atomic<bool>* done;
	uint64 target = 0;

	void Signal();

	void Wait( const std::memory_order order = std::memory_order_relaxed );

	Fence();
	Fence( const Fence& other );
	Fence( Fence&& other );

	Fence( FenceMain& other );

	void operator=( const Fence& other );
};

#endif // FENCE_H