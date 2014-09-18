#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define max(A, B) ((A) >= (B)?(A):(B))

int main(void)
{
	
	int fd, addrlen, newfd, afd;
	int n;
	
	fd_set rfds;

	enum
	{
		idle, busy
	}state;

	int maxfd, counter;

	char *ptr, buffer[128];

	struct sockaddr_in addr;

	if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		exit(1); // error
	}

	memset((void*)&addr,(int) '\0',sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(59000);

	if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
		exit(1); // error
	}

	state = idle;

	while(1)
	{
		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);

		maxfd = fd;

		if(state == busy)
		{
			FD_SET(afd, &rfds);

			maxfd = max(maxfd, afd);
		}

		counter = select(maxfd + 1, &rfds, (fd_set*)NULL, (fd_set*)NULL, (struct timeval *)NULL);

		if(counter <= 0)
		{
			exit(1); //error
		}

		for( ;counter; counter--)
		{
			if(FD_ISSET(fd, &rfds))
			{
				addrlen = sizeof(addr);

				if((newfd = accept(fd, (struct sockaddr*)&addr, &addrlen)) == -1)
				{
					exit(1); //error
				}

				switch(state)
				{
					case idle:
					{
						afd = newfd;
						state = busy;

						break;
					}

					case busy:
					{
						printf("BUSY\n");
						close(newfd);

						break;
					}
				}
			}

			else if(FD_ISSET(afd, &rfds))
			{
				if((n = read(afd, buffer, 128)) != 0)
				{
					if(n == -1)
					{
						exit(1); // error
					}

					printf("Write buffer in AFD: %d\n", &afd);
				}

				else
				{
					close(afd); // connection closed by peer
					state = idle;
				}
			}
		}
	}

	close(fd);
	exit(0);

}