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
	struct sockaddr_in6 si_me, si_other;
	int slen=sizeof(struct sockaddr_in6);
	int n;
	char client_addr_ipv6[100];


	if (argc < 2) {
		fprintf(stderr, "Usage: %s <listening port>\n", argv[0]);
		exit(0);
	}

	printf("\nIPv6 UDP Server Started...\n");

	//Sockets Layer Call: socket()	
	sockfd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd < 0)
		error("ERROR opening socket");
	bzero((char *) &si_me, sizeof(si_me));
	portno = atoi(argv[1]);


	struct in6_addr s6 = { };
	if (!IN6_IS_ADDR_UNSPECIFIED(&s6))
  	inet_pton(AF_INET6, "fe80::2e1:ff:fe00:119f", &s6);

	si_me.sin6_family = AF_INET6;
	si_me.sin6_addr = s6;
	si_me.sin6_port = htons(portno);
	si_me.sin6_flowinfo = 0;

	
	//Sockets Layer Call: bind()
	if (bind(sockfd, (struct sockaddr *) &si_me, sizeof(si_me)) < 0)
		error("ERROR on binding");

	
	//Sockets Layer Call: recvfrom()
	n = recvfrom(sockfd, buffer, 255, 0, (struct sockaddr *) &si_other, &slen);
	if (n < 0)
		error("ERROR reading from socket");

	//Sockets Layer Call: inet_ntop()
	inet_ntop(AF_INET6, &(si_other.sin6_addr),client_addr_ipv6, 100);
	printf("Incoming connection from client having address: %s\n",client_addr_ipv6);
	
	printf("Message from client: %s\n", buffer);

	//Sockets Layer Call: close()
	close(sockfd);
	
	return 0;
}
