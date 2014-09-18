#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

int main(void)
{
	
	int fd, newfd, addrlen;
	int n, nw, ret;

	struct sockaddr_in addr;

	char *ptr, buffer[128];

	pid_t pid;

	void(*old_handler)(int); //interrupt handler

	if((old_handler = signal(SIGCHLD, SIG_IGN)) == SIG_ERR)
	{
		exit(1); // error
	}

	if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		exit(1); // error
	}

	memset((void*)&addr, (int) '\0', sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(59000);

	if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
		exit(1); // error
	}

	if(listen(fd, 2) == -1)
	{
		exit(1); // error
	}

	while(1)
	{
		addrlen = sizeof(addr);

		do newfd = accept(fd, (struct sockaddr*)&addr, &addrlen);

		while(newfd == -1 && errno == EINTR);

		if(newfd == -1)
		{
			exit(1); // error
		}

		if((pid == fork()) == -1)
		{
			exit(1); // error
		}

		else if(pid == 0) // child process
		{
			close(fd);

			while((n = read(newfd, buffer, 128)) != 0)
			{
				if(n == -1)
				{
					exit(1); // error
				}

				ptr = &buffer[0];

				while(n > 0)
				{
					if((nw = write(newfd, ptr, n)) <= 0)
					{
						exit(1); // error
					}

					n -= nw;
					ptr += nw;
				}

			}

			close(newfd);
			exit(0);
		}

		do ret = close(newfd);

		while(ret == -1 && errno == EINTR);

		if(ret == -1)
		{
			exit(1);
		}
	}

	close(fd);
	exit(0);

}