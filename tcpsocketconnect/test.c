#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(void)
{
	int fd, n, addrlen;
	
	struct sockaddr_in addr;
	
	fd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
	if(fd == 1)
	{
		exit(1); // error
	}
	
	memset((void*)&addr, (int)'\0', sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = "localhost";
	addr.sin_port = htons(5800);
	
	n = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
	
	if (n == 1)
	{
		exit(1); // error
	}
	
	write(1, "echo: ", 6); // stdout
	write(1, buffer, n);
	
	close(fd);
	exit(0);
}
