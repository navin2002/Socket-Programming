Networking with C

# Listing Adapters in Linux
##### Required Libraries : 
- <netdb.h>
- <ifaddrs.h>
- <sys/socket.h>

##### Requried Functions : 
```
int getifaddrs(struct ifaddrs **ifap);
``` 
Here *`ifap`* is a pointer to a linked list of data type struct ifaddrs.
Here the link list contains the network interfaces on the system.
The function returns **0** on **success** and **-1** on **error**.

```
void freeifaddrs(struct ifaddrs *ifa);
```
The data returned by getifaddrs() is freed by calling freeifaddrs().
##### Structures Used :
```
//<sys/socket.h>
	struct sockaddr {
	sa_family_t sa_family; // Address family
	char sa_data[]; // Socket address ( variable length data)
}

struct sockaddr_in{
 	sa_family_t sin_family; // address family : AF_INET
 	in_port_t sin_port; // port in network byte order
 	struct in_addr sin_addr;
}

struct in_addr{
	uint32_t s_addr; // address in network byte order
}
```

**sa_family** : AF_INET, AF_UNIX, AF_NS, AF_IMPLINK

```
//ifaddrs.h
struct ifaddrs {
	struct ifaddrs *ifa_next; // Pointer to the next interface in list.
	char *ifa_name; //Name of the network interface
	unsigned int ifa_flag;
	struct sockaddr *ifa_addr; // Network address of this device
	struct sockaddr *ifa_netmask; // Network mask of this device
	union{
	/*
	Either one can happen:
	If IFF_BROADCAST bit is set in 'ifa_flags' then 'ifa_broadaddr' is valid.
	If IFF_POINTOPOINT bit is set then 'ifa_dstaddrs' is valid.
	*/
		struct sockaddr *ifu_broadaddr;
		struct sockaddr *ifu_dstaddr;
	}ifa_ifu;
	/* 
	Definiton also present in <net/if.h> so if they are present already ,no issue.
	*/
	#ifndef ifa_broadaddr
	# define ifa_broadaddr ifa_ifu.ifu_broadaddr
	#endif
	#ifndef ifa_dstaddr
	# define ifa_dstaddr ifa_ifu.ifu_dstaddr
	#endif
	
	void *ifa_data; // Address specific data
}
```
# Listing Adapters in Windows
