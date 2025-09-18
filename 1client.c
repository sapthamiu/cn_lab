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
	int n, a, b;
	char op;
	float res;
	struct sockaddr_in addr;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(PORTNO);
	int status = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
	if(status == -1){
		perror("\nCLIENT ERROR");
		exit(1);
	}
	printf("\nEnter a op b: ");
	scanf("%d%c%d", &a, &op, &b);
	send(sockfd, &a, sizeof(a), 0);
	send(sockfd, &op, sizeof(op), 0);
	send(sockfd, &b, sizeof(b), 0);
	recv(sockfd, &res, sizeof(res), 0);
	printf("Result: %.2f\n", res);
	close(sockfd);
	return 0;
}