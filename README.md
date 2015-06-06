# net-adapter
cross platform library for getting presentable Network Adapter information. Written in a "c style" C++.

Network adapter names (e.g. "Wireless Network Connection", "eth0", "en0", or "em0")

* IP address (e.g. 192.168.0.184)
* Subnet Mask (e.g. 255.255.255.0)
* UDP Broadcast address (e.g. 192.168.0.255)
* MAC address (e.g. b0:90:92:01:e2:aa)

The goal of the library is to provide presentable data, so data is stored as c strings. For use, data is also available as arrays of unsigned chars and unsigned shorts (where applicable).

Usage (Primary Adapter):

```c++
#include <NetAdapter/NetAdapter.h>                                              // Or wherever you put it //

// ... //

pNetAdapterInfo* Adapters = new_pNetAdapterInfo();                              // Many Adapters (hence the "p") //

const NetAdapterInfo* Current = get_primary_pNetAdapterInfo( Adapters );        // One Adapter (no "p") //
printf( "%s: %s (%s)\n", Current->Name, Current->IP, Current->MAC );

delete_pNetAdapterInfo( Adapters );                                             // Cleanup //
```

Usage (All Adapters):

```c++
pNetAdapterInfo* Adapters = new_pNetAdapterInfo();

size_t AdapterCount = count_pNetAdapterInfo(Adapters);                          // Count the Adapters //
for ( size_t Index = 0; Index < AdapterCount; Index++ ) {
        const NetAdapterInfo* Current = get_pNetAdapterInfo( Adapters, Index ); // get_, not get_primary_ //
        printf( "%i - %s: %s (%s)\n", Index, Current->Name, Current->IP, Current->MAC );
}

delete_pNetAdapterInfo( Adapters );
```

Usage (Printing an IP address using the raw data):

```c++
const NetAdapterInfo* Ct = get_primary_pNetAdapterInfo( Adapters );
printf( "%s: %i.%i.%i.%i\n", Ct->Name, Ct->Data.IP[0], Ct->Data.IP[1], Ct->Data.IP[2], Ct->Data.IP[3] );
```
More information can be found in [NetAdapter.h](NetAdapter/NetAdapter.h)


# History

Way back in 2012, I wanted a simple way to get the IP and Mac address of the current computer. 

Since Google Code is shutting down, I moved it. Here is the original blog post:

```
# Net Adapter – AKA the “Get IP/MAC address” library
So hey. I wrote and open sourced a small cross platform library for getting 
the IP and MAC address of the current device. You can find it on Google Code.

http://code.google.com/p/net-adapter/

So far I’ve tested it on Windows, Linux (Ubuntu), and Mac OSX Mountain Lion.
It *should* also work on iOS, BSD and Android. If you happen to test it on 
one of those other platforms, I would love to know what the Sample program 
outputs. Thanks!
```
http://www.toonormal.com/2012/10/26/net-adapter-aka-the-get-ipmac-address-library/
