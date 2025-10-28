#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>

#define MAX 1024
#define PORTNO 14201
int main(){
    char buf[MAX];
    struct sockaddr_in seraddr;
    int clientfd;

    if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
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

    printf("Connected to server\n");

    while(1){
        printf("Enter message: ");
        memset(buf, 0, MAX);  
        fgets(buf, sizeof(buf), stdin);
        int n;
        if((n = send(clientfd, buf, strlen(buf), 0)) > 0){
            printf("Sent: %s\n", buf);  
            if(strncmp(buf, "end", 3) == 0){
                printf("Terminating\n");
                break;
            }
            memset(buf, 0, MAX);  
            int n;
            if((n = recv(clientfd, buf, MAX-1, 0)) > 0){
                buf[n] = '\0';  
                printf("Message from server: %s\n", buf);
            }
            else{
                perror("recv failed");
                break;
            }
        }
    }
    close(clientfd);
    return 0;
}