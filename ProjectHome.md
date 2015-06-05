A cross platform library for getting presentable Network Adapter information. Written in a "c style" C++.

  * Network adapter names (e.g. "Wireless Network Connection", "eth0", "en0", or "em0")
  * IP address (e.g. 192.168.0.184)
  * Subnet Mask (e.g. 255.255.255.0)
  * UDP Broadcast address (e.g. 192.168.0.255)
  * MAC address (e.g. b0:90:92:01:e2:aa)

The goal of the library is to provide presentable data, so data is stored as c strings. For use, data is also available as arrays of unsigned chars and unsigned shorts (where applicable).

Usage (Primary Adapter):
```
#include <NetAdapter/NetAdapter.h>						// Or wherever you put it //

// ... //

pNetAdapterInfo* Adapters = new_pNetAdapterInfo();				// Many Adapters (hence the "p") //

const NetAdapterInfo* Current = get_primary_pNetAdapterInfo( Adapters );	// One Adapter (no "p") //
printf( "%s: %s (%s)\n", Current->Name, Current->IP, Current->MAC );

delete_pNetAdapterInfo( Adapters );						// Cleanup //
```

Usage (All Adapters):
```
pNetAdapterInfo* Adapters = new_pNetAdapterInfo();

size_t AdapterCount = count_pNetAdapterInfo(Adapters);				// Count the Adapters //
for ( size_t Index = 0; Index < AdapterCount; Index++ ) {
	const NetAdapterInfo* Current = get_pNetAdapterInfo( Adapters, Index );	// get_, not get_primary_ //
	printf( "%i - %s: %s (%s)\n", Index, Current->Name, Current->IP, Current->MAC );
}

delete_pNetAdapterInfo( Adapters );
```

Usage (Printing an IP address using the raw data):
```
const NetAdapterInfo* Ct = get_primary_pNetAdapterInfo( Adapters );
printf( "%s: %i.%i.%i.%i\n", Ct->Name, Ct->Data.IP[0], Ct->Data.IP[1], Ct->Data.IP[2], Ct->Data.IP[3] );
```

More information can be found in [NetAdapter.h](https://code.google.com/p/net-adapter/source/browse/trunk/NetAdapter/NetAdapter.h)