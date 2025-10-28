//Write a UDP client server program where client sends a message 
//to server and server echoes back the message to client. 
//Client should display the original message and echoed message.
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

#define MAX 1024
#define PORTNO 14201
int main(){
    struct sockaddr_in seraddr, cliaddr;
    char buf[MAX];
    int listenfd;

    if((listenfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket creation failed");
        exit(1);
    }

    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    seraddr.sin_port = htons(PORTNO);

    if(bind(listenfd, (struct sockaddr*)&seraddr, sizeof(seraddr)) < 0){
        perror("bind failed");
        exit(1);
    }

    printf("Server listening on port %d\n", PORTNO);

    while(1){
        socklen_t clilen = sizeof(cliaddr);
        int n;
        memset(buf, 0, MAX);
        if((n = recvfrom(listenfd, buf, MAX, 0, (struct sockaddr*)&cliaddr, &clilen)) > 0){
            if(strncmp(buf, "end", 3) == 0){
                printf("Terminating client\n");
                break;
            }
            buf[n] = '\0';
            printf("Received: %s\n", buf);
            sendto(listenfd, buf, n, 0, (struct sockaddr*)&cliaddr, clilen);
            printf("Sent: %s\n", buf);
        }
    }
    close(listenfd);
    return 0;
}