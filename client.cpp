#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <libc.h>
#include <iostream> 

int main()
{
	unsigned int		maxLen = 1024;
	char				message[maxLen];
    int					sock;
    struct				sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }
	while (1)
	{
		std::cin.getline(message, maxLen);
		if(strlen(message) > 0)
		{
			send(sock, message, strlen(message) + 1, 0);
			if (strcmp(message, "STOP") == 0)
				break ;
		}
	}
	close(sock);
	return 0;
}
