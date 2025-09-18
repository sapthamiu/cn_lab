#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>

#define PORTNO 10200
int main(){
	int n;
	struct sockaddr_in addr;
	char ch[256], buf[256];
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(PORTNO);
	int result = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
	if(result == -1){
		perror("\nCLIENT ERROR");
		exit(1);
	}
	while(1){
		printf("\nENTER STRING(Stop to quit):\t");
		fgets(ch, sizeof(ch), stdin);
		ch[strcspn(ch, "\n")] = '\0';
		send(sockfd, ch, strlen(ch), 0);
		if(strcmp(ch, "Stop") == 0){
			printf("Terminating client\n");
			break;
		}
		printf("STRING SENT BACK FROM SERVER: ");
		n = recv(sockfd, buf, sizeof(buf), 0);
		if(n <= 0){
			printf("Server closed connection");
			break;
		}
		buf[n] = '\0';
		puts(buf);
	}
	close(sockfd);
	return 0;
}