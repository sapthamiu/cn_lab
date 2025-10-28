#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#define MAX 100
#define PORTNO 14201
int main(){
    char str[MAX];
    char result[MAX];
    struct sockaddr_in seraddr;
    int clientfd, n;

    if((clientfd=socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket creation failed");
        exit(1);
    }

    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(PORTNO);
    seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(clientfd, (struct sockaddr*)&seraddr, sizeof(seraddr)) < 0){
        perror("connection failed");
        exit(1);
    }
    while(1){
        memset(str, 0, MAX);
        printf("Enter a sentence: ");
        fgets(str, MAX, stdin);
        str[strcspn(str, "\n")] = '\0';
        if(send(clientfd, str, strlen(str), 0) > 0){
            if(strcmp(str, "Stop") == 0){
                printf("Terminating\n");
                break;
            }
            memset(result, 0, MAX);
            if((n = recv(clientfd, result, sizeof(result), 0))>0){
                printf("Received: %s\n", result);
            }
        }
    }
    close(clientfd);
    return 0;
}