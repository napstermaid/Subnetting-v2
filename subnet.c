#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "subnet.h"

int host;
int network=0x0,broadcast=0x0;
int numbers=0x1;

void help() {
	puts("Options: ");
	puts("\t#--classC or -cC");
	puts("\t\tPrefix: 24 - 30");
	puts("\t#--classB or -cB");
	puts("\t\tPrefix: 16 - 23, 24 - 30");
	puts("\t#--classA or -cA");
	puts("\t\tPrefix: 8 - 15, 16 - 23, 24 - 30");
	puts("Examples:");
	puts("IP address Class C: ./subnet 192.168.10.1/24 --classC");
	puts("IP address Class B: ./subnet 172.16.10.22/16 --classB");
	puts("IP address Class A: ./subnet 10.10.1.1/8 --classA");
}

void moveBuffer(char *buffer, ipaddr_t *ip) {
	char *address = (char*)malloc(0x40*sizeof(char));
        strcpy(address, buffer);
        char tmp[0x3];
        int y=0x0, x=0x0;

        for (int i=0x0;i<strlen(address);i++) {
                if ((address[i] != __DOT_) && (address[i] != __PREFIX_)) {                                                                                          tmp[x] = address[i];                                                  x++;
                }
                else if ((address[i] == __DOT_) || (address[i] == __PREFIX_)) {                                                                                     ip->octet[y] = atoi(tmp);
                        memset(tmp,0x00,sizeof(tmp));
                        x=0x0;
                        y++;
                        if (address[i] == __PREFIX_) {
                                for (int j=(i+0x1);j<=strlen(address);j++) {
                                        tmp[x] = address[j];
                                        x++;
                                }
                                ip->prefix = atoi(tmp);
                                free(address);
                                break;
                        }
                }
        }	
}

void prefix24_30(ipaddr_t *ip) {
	object_t obj;

	obj.grade[0x0] = 0x20 - ip->prefix;
	obj.subnet[0x0] = pow (2, obj.grade[0x0]);
	obj.mask = 0x100 - obj.subnet[0x0];

	printf("IP Address: %d.%d.%d.%d\n", ip->octet[0x0], ip->octet[0x1], ip->octet[0x2], ip->octet[0x3]);
	printf("Subnetmask: %d.%d.%d.%d\n", 0xff, 0xff, 0xff, obj.mask);
	host = obj.subnet[0x0] - 0x2;
	printf("Host available: %d\n", host);

	LINE
	printf(" Network\t\t Broadcast\n");
	do {
		printf("%d.%d.%d."BOLDRED"%d\t"WHT, ip->octet[0x0], ip->octet[0x1], ip->octet[0x2], network);
		network += obj.subnet[0x0];
		broadcast = network - 0x1;
		printf("  -\t%d.%d.%d."BOLDRED"%d\t"WHT"->[%d]\n", ip->octet[0x0], ip->octet[0x1], ip->octet[0x2], broadcast, numbers);
		numbers++;
	} while(network <=0xff);
	LINE
}

void prefix16_23(ipaddr_t *ip) {
	object_t obj;

	obj.grade[0x0] = 0x20 - ip->prefix;
	obj.grade[0x1] = 0x18 - ip->prefix;
	obj.subnet[0x0] = pow (2, obj.grade[0x0]);
	obj.subnet[0x1] = pow (2, obj.grade[0x1]);
	obj.mask = 0x100 - obj.subnet[0x1];

	printf("IP Address: %d.%d.%d.%d\n", ip->octet[0x0], ip->octet[0x1], ip->octet[0x2], ip->octet[0x3]);
	printf("Subnetmask: %d.%d.%d.%d\n", 0xff, 0xff, obj.mask, 0x0);
	host = obj.subnet[0x0] - 0x2;
	printf("Host available: %d\n", host);

	LINE
	printf(" Network\t\t Broadcast\n");
	do {
		printf("%d.%d."BOLDRED"%d.%d\t"WHT, ip->octet[0x0], ip->octet[0x1], network, 0x0);
		network += obj.subnet[0x1];
		broadcast = network - 0x1;
		printf("  -\t%d.%d."BOLDRED"%d.%d\t"WHT"->[%d]\n", ip->octet[0x0], ip->octet[0x1], broadcast, 0xff, numbers);
		numbers++;
	} while(network <= 0xff);
	LINE
}

void prefix8_15(ipaddr_t *ip) {
	object_t obj;

	obj.grade[0x0] = 0x20 - ip->prefix;
	obj.grade[0x1] = 0x10 - ip->prefix;
	obj.subnet[0x0] = pow (2, obj.grade[0x0]);
	obj.subnet[0x1] = pow (2, obj.grade[0x1]);
	obj.mask = 0x100 - obj.subnet[0x01];

	printf("IP Address: %d.%d.%d.%d\n", ip->octet[0x0], ip->octet[0x1], ip->octet[0x2], ip->octet[0x3]);
	printf("Subnetmask: %d.%d.%d.%d\n", 0xff, obj.mask, 0x0, 0x0);
	host = obj.subnet[0x0] - 0x2;
	printf("Host available: %d\n", host);

	LINE
	printf(" Network\t\t Broadcast\n");
	do {
		printf("%d."BOLDRED"%d.%d.%d\t"WHT, ip->octet[0x0], network, 0x0, 0x0);
		network += obj.subnet[0x1];
		broadcast = network - 0x1;
		printf(" -\t%d."BOLDRED"%d.%d.%d\t"WHT"->[%d]\n", ip->octet[0x0], broadcast, 0xff, 0xff, numbers);
		numbers++;
	} while(network <= 0xff);
	LINE
}
