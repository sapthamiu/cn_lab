#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
float calculate(int a, char op, int b){
	switch(op){
		case '+': return a+b;
		case '-': return a-b;
		case '*': return a*b;
		case '/': return b!= 0 ? a/b : 99999.99;
		default: printf("Check again");			
	}
}
#define PORTNO 10200
int main(){
	int n, a, b;
	char op;
	struct sockaddr_in seraddr, cliaddr;
	float res;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	seraddr.sin_port = htons(PORTNO);

	bind(sockfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
	listen(sockfd, 5);

	while(1){
		int clilen = sizeof(clilen);
		int newsockfd = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen);
		if(fork() == 0){
			//child process
			recv(newsockfd, &a, sizeof(a), 0);
			recv(newsockfd, &op, sizeof(op), 0);
			recv(newsockfd, &b, sizeof(b), 0);
			printf("\n[Child %d] Received: %d %c %d\n", getpid(), a, op, b);
			sleep(10); 
			res = calculate(a, op, b);
			send(newsockfd, &res, sizeof(res), 0);
			printf("Sent: %.2f", res);
			close(newsockfd);
		}
		else close(newsockfd);
	}
	close(sockfd);
	return 0;
}