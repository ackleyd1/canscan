#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>

#include<net/if.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/ioctl.h>

#include<linux/can.h>
#include<linux/can/raw.h>


int main(int argc, int* argv[]) {
	int s;
	struct sockaddr_can addr;
	struct ifreq ifr;
	// struct ArbidNode arbids[2048];

	s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if(s == -1) {
		return -1;
	}
	strcpy(ifr.ifr_name, "can0");
	ioctl(s, SIOCGIFINDEX, &ifr);

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if(bind(s, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		return -2;
	}

	struct can_frame frame;
	while(true) {
		size_t nbytes = read(s, &frame, sizeof(struct can_frame));
		if ( nbytes ) {
			// TODO
		}
	}	
}

