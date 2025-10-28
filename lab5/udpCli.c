#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#define MAX 1024
#define PORTNO 14201

int main(){
    struct sockaddr_in seraddr;
    int clientfd;
    char buf[MAX];
    if((clientfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket creation failed");
        exit(1);
    }
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(PORTNO);
    seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while(1){
        int n;
        socklen_t serlen = sizeof(seraddr);
        memset(buf, 0, MAX);
        printf("Enter message: ");
        fgets(buf, MAX, stdin);
        if((n = sendto(clientfd, buf, strlen(buf), 0, (struct sockaddr*)&seraddr, serlen))> 0){
            if(strncmp(buf, "end", 3) == 0){
                printf("Terminating.\n");
                break;
            }
            printf("Sent: %s\n", buf);
            memset(buf, 0, MAX);
            n = recvfrom(clientfd, buf, MAX, 0, (struct sockaddr*)&seraddr, &serlen);
            buf[n] = '\0';
            printf("Received: %s\n", buf);
        }
    }
    close(clientfd);
}