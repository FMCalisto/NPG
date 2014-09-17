#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	int fd, nbytes, nleft, nwritten, nread;

	char *ptr, buffer[128];
	
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

	/*
	
	n = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
	
	if (n == 1)
	{
		exit(1); // error
	}

	*/

	ptr = strcpy(buffer, "Hello!\n");
	nbytes = 7;

	nleft = nbytes;

	while(nleft > 0)
	{
		nwritten = write(fd, ptr, nleft);

		if(nwritten <= 0)
		{
			exit(1); // error
		}

		nleft -= nwritten;
		ptr += nwritten;
	}

	nleft = nbytes;
	ptr = &buffer[0];

	while(nleft > 0)
	{
		nread = write(fd, ptr, nleft);

		if(nread == -1)
		{
			exit(1); // error
		}
		else if(nread == 0)
		{
			break; // closed by peer
		}

		nleft -= nread;
		ptr += nread;
	}

	nread = nbytes - nbytes;
	close(fd);
	
	write(1, "echo: ", 6); // stdout
	write(1, buffer, nread);
	
	exit(0);
}
