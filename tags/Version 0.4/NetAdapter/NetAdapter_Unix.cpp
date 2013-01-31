// - ------------------------------------------------------------------------------------------ - //
// NET ADAPTER -- A cross platform library for getting presentable Network Adapter information.
// By Mike Kasprzak -- http://www.toonormal.com http://www.sykhronics.com -- twitter @mikekasprzak
// http://code.google.com/p/net-adapter/
//
// This is free and unencumbered software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software, either
// in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and
// by any means.
// 
// In jurisdictions that recognize copyright laws, the author or authors of this software dedicate
// any and all copyright interest in the software to the public domain. We make this dedication for
// the benefit of the public at large and to the detriment of our heirs and successors. We intend 
// this dedication to be an overt act of relinquishment in perpetuity of all present and future 
// rights to this software under copyright law.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// 
// For more information, please refer to <http://unlicense.org/>
// - ------------------------------------------------------------------------------------------ - //
#if !defined(NET_ADAPTER_STUB)
#if (defined(__unix__) && !defined(__CYGWIN__)) || defined(__APPLE__) || defined(__QNXNTO__)
// - ------------------------------------------------------------------------------------------ - //
#if defined(__linux__) || defined(__ANDROID__)
#define _LINUX_PATH
#else // defined(__APPLE__) || defined(__bsdi__) || defined(__unix__) // Other Unix //
#define _UNIX_PATH
#endif // defined(__linux__) || defined(__APPLE__)
// - ------------------------------------------------------------------------------------------ - //
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>				// IFF_BROADCAST
#include <netdb.h>
// - ------------------------------------------------------------------------------------------ - //
#ifdef _LINUX_PATH
#include <linux/if_packet.h>	// sockaddr_ll
#else // _UNIX_PATH //
#include <net/if_dl.h>			// sockaddr_dl
#endif // _LINUX_PATH //
// - ------------------------------------------------------------------------------------------ - //
// http://www.kernel.org/doc/man-pages/online/pages/man3/getifaddrs.3.html
// http://stackoverflow.com/questions/6762766/mac-address-with-getifaddrs
// http://stackoverflow.com/questions/3964494/having-a-problem-figuring-out-how-to-get-ethernet-interface-info-on-mac-os-x-usi
// http://freebsd.active-venture.com/FreeBSD-srctree/newsrc/net/if_dl.h.html
// - ------------------------------------------------------------------------------------------ - //
#include "NetAdapter.h"
#include "NetAdapter_Internal.h"
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
#include <stdio.h>
#include <stdlib.h>
// - ------------------------------------------------------------------------------------------ - //
#define safe_sprintf snprintf
// - ------------------------------------------------------------------------------------------ - //


// - ------------------------------------------------------------------------------------------ - //
pNetAdapterInfo* new_pNetAdapterInfo() {
	ifaddrs* IFA;
	if ( getifaddrs( &IFA ) == 0 ) {

		// Count the number Interfaces with IPv4 addresses //
		size_t IPv4Count = 0;
		for( ifaddrs* Current = IFA; Current != 0; Current = Current->ifa_next ) {
			// If an IPv4 //
			if ( Current->ifa_addr->sa_family == AF_INET ) {
				IPv4Count++;
			}
		}
		
		// *** //
		
		// Allocate the pNetAdapterInfo's //
		pNetAdapterInfo* Adapters = new pNetAdapterInfo[IPv4Count+1];
	
		// Allocate NetAdapterInfos per IP //	
		for ( size_t idx = 0; idx < IPv4Count; idx++ ) {
			Adapters[idx] = new_NetAdapterInfo();	// Internal Function //
		}
		
		Adapters[IPv4Count] = 0; // Last one a null pointer //

		// *** //
		
		// Iterate though and populate data -- Note: this used to use getnameinfo. It doesn't anymore. //
		size_t Index = 0;
		for( ifaddrs* Current = IFA; Current != 0; Current = Current->ifa_next ) {
			// If an IPv4 //
			if ( Current->ifa_addr->sa_family == AF_INET ) {
				// Copy IP as byte array //
				sockaddr_in* SAI = (sockaddr_in*)Current->ifa_addr;
				const unsigned char* DataAddr = (const unsigned char*)&(SAI->sin_addr.s_addr);

				int* IPv4 = (int*)Adapters[Index]->Data.IP;
				*IPv4 = *(int*)DataAddr;
			
				// Copy IP as string //
				//getnameinfo( Current->ifa_addr, sizeof(struct sockaddr_in), Adapters[Index]->IP, NI_MAXHOST, NULL, 0, NI_NUMERICHOST );
				safe_sprintf( Adapters[Index]->IP, sizeof(Adapters[Index]->IP), "%s", inet_ntoa( SAI->sin_addr ) );
			
//				printf("%i.%i.%i.%i\n", Adapters[Index]->Data.IP[0], Adapters[Index]->Data.IP[1], Adapters[Index]->Data.IP[2], Adapters[Index]->Data.IP[3] );
			
				// Copy Name //
				safe_sprintf( Adapters[Index]->Name, sizeof(Adapters[Index]->Name), "%s", Current->ifa_name );
				
				// Copy NetMask as string //
				//getnameinfo( Current->ifa_netmask, sizeof(struct sockaddr_in), Adapters[Index]->NetMask, NI_MAXHOST, NULL, 0, NI_NUMERICHOST );
				sockaddr_in* SNI = (sockaddr_in*)Current->ifa_netmask;
				safe_sprintf( Adapters[Index]->NetMask, sizeof(Adapters[Index]->NetMask), "%s", inet_ntoa( SNI->sin_addr ) );
			
				// Copy Broadcast Address //
	            if ( Current->ifa_flags & IFF_BROADCAST ) {
					//getnameinfo( Current->ifa_broadaddr, sizeof(struct sockaddr_in), Adapters[Index]->Broadcast, NI_MAXHOST, NULL, 0, NI_NUMERICHOST );
					sockaddr_in* SBI = (sockaddr_in*)Current->ifa_broadaddr;
					safe_sprintf( Adapters[Index]->Broadcast, sizeof(Adapters[Index]->Broadcast), "%s", inet_ntoa( SBI->sin_addr ) );
				}
				
				//if ( Current->ifa_flags & IFF_POINTOPOINT ) {
				//	//getnameinfo( Current->ifa_dstaddr, sizeof(struct sockaddr_in), Adapters[Index]->Broadcast, NI_MAXHOST, NULL, 0, NI_NUMERICHOST );					
				//	sockaddr_in* SPI = (sockaddr_in*)Current->ifa_dstaddr;
				//	safe_sprintf( Adapters[Index]->Point2Point, sizeof(Adapters[Index]->Point2Point), "%s", inet_ntoa( SPI->sin_addr ) );
				//}
				
				Index++;	
			}
		}
		
		// 2nd pass, extract other data (MAC Address, IPv6) //
		Index = 0;
		for( ifaddrs* Current = IFA; Current != 0; Current = Current->ifa_next ) {
#ifdef _LINUX_PATH
			// If an AF_PACKET device (i.e. hardware device) //
			if ( Current->ifa_addr->sa_family == AF_PACKET ) {
				for( int idx=0; idx < IPv4Count; idx++ ) {
					if ( strcmp( Adapters[idx]->Name, Current->ifa_name ) == 0 ) {
						sockaddr_ll* s = (sockaddr_ll*)Current->ifa_addr;

						// NOTE: I'm assuming MAC address is 6 bytes long //
						memcpy( Adapters[idx]->Data.MAC, s->sll_addr, 6 );
						
						safe_sprintf( Adapters[idx]->MAC, sizeof(Adapters[idx]->MAC), "%02x:%02x:%02x:%02x:%02x:%02x",
							s->sll_addr[0],
							s->sll_addr[1],
							s->sll_addr[2],
							s->sll_addr[3],
							s->sll_addr[4],
							s->sll_addr[5]
							);
					}
				}
			}
#else // _UNIX_PATH //
			if ( Current->ifa_addr->sa_family == AF_LINK ) {
				for( int idx=0; idx < IPv4Count; idx++ ) {
					if ( strcmp( Adapters[idx]->Name, Current->ifa_name ) == 0 ) {
						sockaddr_dl* s = (sockaddr_dl*)Current->ifa_addr;

						memcpy( Adapters[idx]->Data.MAC, LLADDR(s), s->sdl_alen );
						
						safe_sprintf( Adapters[idx]->MAC, sizeof(Adapters[idx]->MAC), "%02x:%02x:%02x:%02x:%02x:%02x",
							Adapters[idx]->Data.MAC[0],
							Adapters[idx]->Data.MAC[1],
							Adapters[idx]->Data.MAC[2],
							Adapters[idx]->Data.MAC[3],
							Adapters[idx]->Data.MAC[4],
							Adapters[idx]->Data.MAC[5]
							);
					}
				}
			}
#endif // _LINUX_PATH //
			
			if ( Current->ifa_addr->sa_family == AF_INET6 ) {
				// IPv6 Address //
			}
		}
		

		freeifaddrs( IFA );

		return Adapters;
	}
	
	return 0;
}
// - ------------------------------------------------------------------------------------------ - //
const NetAdapterInfo* get_primary_pNetAdapterInfo( const pNetAdapterInfo* Adapters ) {
	if ( Adapters ) {
		NetAdapterInfo* NotLocalHost = 0;
		static unsigned char LocalHostIPv4[] = {127,0,0,1};
		
		size_t Index = 0;
		while ( Adapters[Index] != 0 ) {
			// Assume "eth0" is the primary (Linux) //
			if ( strcmp( Adapters[Index]->Name, "eth0" ) == 0 )
				return Adapters[Index];

			// Assume "en0" is the primary (OSX, iOS) //
			if ( strcmp( Adapters[Index]->Name, "en0" ) == 0 )
				return Adapters[Index];

			// Assume "em0" is the primary (BSD) //
			if ( strcmp( Adapters[Index]->Name, "em0" ) == 0 )
				return Adapters[Index];
			
			// If none of the above, then assume the first non LocalHost address is primary //
			if ( NotLocalHost == 0 ) {
				int* a = (int*)LocalHostIPv4;
				int* b = (int*)Adapters[Index]->Data.IP;
				if ( *a != *b ) {
					NotLocalHost = Adapters[Index];
				}
			}
			
			Index++;
		};
		
		return NotLocalHost;
	}
	else {
		// Hey you goof! You gave me a null pointer! //
		return 0;
	}
}
// - ------------------------------------------------------------------------------------------ - //
#endif // defined(__unix__) && !defined(__CYGWIN__) || defined(__APPLE__) || defined(__QNXNTO__) //
#endif // !NET_ADAPTER_STUB //
// - ------------------------------------------------------------------------------------------ - //
