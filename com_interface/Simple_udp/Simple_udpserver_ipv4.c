#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <arpa/inet.h>

//Based on http://www.abc.se/~m6695/udp.html

void error(char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]) {
	int sockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in si_me, si_other;
	int slen = sizeof(struct sockaddr_in);
	int n;



	if (argc < 2) {
		fprintf(stderr, "Usage: %s <listening port>\n", argv[0]);
		exit(0);
	}

	printf("\nIPv4 UDP Server Started...\n");

	//Sockets Layer Call: socket()	
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd < 0)
		error("ERROR opening socket");
	bzero((char *) &si_me, sizeof(si_me));
	portno = atoi(argv[1]);

	si_me.sin_family = AF_INET;
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	si_me.sin_port = htons(portno);


	
	//Sockets Layer Call: bind()
	if (bind(sockfd, (struct sockaddr *) &si_me, sizeof(si_me)) < 0)
		error("ERROR on binding");

	
	//Sockets Layer Call: recvfrom()
	n = recvfrom(sockfd, buffer, 255, 0, (struct sockaddr *) &si_other, &slen);
	if (n < 0)
		error("ERROR reading from socket");

	//Sockets Layer Call: inet_ntoa()

	printf("Incoming connection from client having IPv4 address: %s\n", inet_ntoa(si_other.sin_addr));
	
	printf("Message from client: %s\n", buffer);

	//Sockets Layer Call: close()
	close(sockfd);
	
	return 0;
}
