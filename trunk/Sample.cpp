
#include <stdio.h>
#include "NetAdapter/NetAdapter.h"

#ifdef _WIN32
#include <Winsock2.h>
WSADATA wsaData;
void InitNetworking() {
	// Init Winsock2 //
	WSAStartup( MAKEWORD(2,2), &wsaData );
	atexit( (void(*)())WSACleanup );
}
#else // _WIN32 //
void InitNetworking() { }
#endif // _WIN32 //

int main( int argc, char* argv[] ) {
	InitNetworking();

	{
		pNetAdapterInfo* Adapters = new_pNetAdapterInfo();							// Get adapters //
		
		const NetAdapterInfo* Current = get_primary_pNetAdapterInfo( Adapters );	// Get primary adapter //
		if ( Current ) {
			printf( "Primary:\n%s: %s/%s [%s] (%s)\n", 
				Current->Name, 
				Current->IP, 
				Current->NetMask, 
				Current->Broadcast, 
				Current->MAC 
				);
		}
		else {
			printf( "No Primary Adapter Found!\n" );
		}

		delete_pNetAdapterInfo( Adapters );											// Clean up //
	}

	printf( "\n" );

	{
		pNetAdapterInfo* Adapters = new_pNetAdapterInfo();

		size_t AdapterCount = count_pNetAdapterInfo(Adapters);
		for ( size_t Index = 0; Index < AdapterCount; Index++ ) {
			const NetAdapterInfo* Current = get_pNetAdapterInfo( Adapters, Index );	// get_, not get_primary_ //
			printf( "%i - %s: %s (%s)\n", (int)Index, Current->Name, Current->IP, Current->MAC );
		}

		delete_pNetAdapterInfo( Adapters );
	}

	return 0;
}
