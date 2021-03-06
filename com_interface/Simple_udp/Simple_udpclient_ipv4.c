#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//Based on http://www.abc.se/~m6695/udp.html

void error(char *msg) {
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[]) {
	int sockfd, portno;
	struct hostent *server;
	char buffer[256] = "This is a string from client!";
	struct sockaddr_in serv_addr;
	int slen = sizeof(struct sockaddr_in);
	int n;
	
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <server adddress> <server port>\n", argv[0]);
		exit(0);
	}
	portno = atoi(argv[2]);

	printf("\nIPv4 UDP Client Started...\n");
	
	//Sockets Layer Call: socket()
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd < 0)
		error("ERROR opening socket");

	//Sockets Layer Call: gethostbyname()
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}

	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);

	memmove((char *) &serv_addr.sin_addr.s_addr, (char *) server->h_addr, server->h_length);

	
	//Sockets Layer Call: sendto()
	n = sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *) &serv_addr, slen);
	if (n < 0)
		error("ERROR writing to socket");

	printf("\nString sent to server...\n");
		
	//Sockets Layer Call: close()
	close(sockfd);
		
	return 0;
}
