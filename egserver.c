#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>

#define PORTNO 10200
int main(){
	int n;
	char buf[256];
	struct sockaddr_in seraddr, cliaddr;
	int i, value;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	seraddr.sin_port = htons(PORTNO);

	bind(sockfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
	listen(sockfd, 5);

	while(1){
		printf("Server waiting");
		int clilen = sizeof(clilen);
		int newsockfd = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen);
		if(fork() == 0){
			//child process
			while(1){
				n = recv(newsockfd, buf, sizeof(buf), 0);
				if(n <= 0){
					printf("Client disconnected");
					break;
				}
				buf[n] = '\0';
				if(strcmp(buf, "Stop") == 0){
					printf("Client requested termination\n");
					break;
				}
				printf("\nMessage from Client: %s\n", buf);
				send(newsockfd, buf, sizeof(buf), 0);
			}
			close(newsockfd);
		}
		else close(newsockfd);
	}
	close(sockfd);
	return 0;
}