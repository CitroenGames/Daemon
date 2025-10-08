#pragma once

namespace Net {

inline const std::string& OOBHeader()
{
	static const std::string header ="\xff\xff\xff\xff";
	return header;
}

template<class... Args>
void OutOfBandPrint( netsrc_t net_socket, const netadr_t& adr, Str::StringRef format, Args&&... args )
{
	std::string message = OOBHeader() + Str::Format( format, std::forward<Args>(args)... );
	NET_SendPacket( net_socket, message.size(), message.c_str(), adr );
}

/*
 * Sends huffman-compressed data
 */
void OutOfBandData( netsrc_t sock, const netadr_t& adr, byte* data, std::size_t len );

/*
 * Converts an address to its string representation
 */
std::string AddressToString( const netadr_t& address, bool with_port = false );

/*
 * Returns port, port4 or port6 depending on the address type
 */
unsigned short GetPort(const netadr_t& address);

// --- ASYNC DNS RESOLUTION ---
// This API wraps the synchronous DNS resolution (NET_StringToAdr),
// running it in a separate thread.

// 0 can be used as an invalid value
using DNSQueryHandle = size_t;

struct DNSResult
{
	netadr_t ipv4;
	netadr_t ipv6;

	DNSResult()
	{
		ipv4.type = netadrtype_t::NA_BAD;
		ipv6.type = netadrtype_t::NA_BAD;
	}
};

DNSQueryHandle AllocDNSQuery();

// The resolver thread will start working on the query and periodically
// refresh it in the background.
// protocolMask considers NET_ENABLEV4 and NET_ENABLEV6
void SetDNSQuery(DNSQueryHandle query, std::string hostname, int protocolMask);

// type == NA_BAD if the query is still running or failed, or for a disabled protocol
DNSResult GetAddresses(DNSQueryHandle query);

// Stop the resolver thread
// You should clear any queries when you do this, so you won't get stale results
void ShutDownDNS();

} // namespace Net

