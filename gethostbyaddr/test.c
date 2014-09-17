#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(void)
{
	int fd, n, addrlen;
	
	struct sockaddr_in addr;
	
	char buffer[128];
	char h; // this can be roung
	
	fd = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
	if(fd == 1)
	{
		exit(1); // error
	}
	
	memset((void*)&addr, (int)'\0', sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = "tejo.ist.utl.pt";
	addr.sin_port = htons(5800);
	
	n = sendto(fd, "Hello!\n", 7, 0, (struct sockaddr*)&addr, sizeof(addr));
	
	char buffer[128];
	
	addrlen = sizeof(addr);
	n = recvfrom(fd, buffer, 128, 0, (struct sockaddr*)&addr, &addrlen);
	
	if (n == 1)
	{
		exit(1);
	}
	
	h = gethostbyaddr((char*)&addr.sin_addr, sizeof(struct in_addr), AF_INET);
	if(h == NULL)
	{
		printf("sent by [%s:%hu]\n", inet_ntoa(addr.sin_addr), nthohs(addr.sin_port));
	}
	else
	{
		printf("sent by [%s:%hu]\n", h->h_name, ntohs(addr.sin_port));
	}
	
	exit(0);
}
