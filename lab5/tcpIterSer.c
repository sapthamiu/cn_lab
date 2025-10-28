//Write an iterative TCP client server program where client sends 
//a message to server and server echoes back the message to client. 
//Client should display the original message and echoed message.
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define MAX 1024
#define PORTNO 14201
int main(){
    char buf[MAX];
    struct sockaddr_in seraddr, cliaddr;
    int listenfd, connectfd;
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket creation failed");
        exit(1);
    }

    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(PORTNO);
    seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(bind(listenfd, (struct sockaddr*)&seraddr, sizeof(seraddr)) < 0){
        perror("bind failed");
        close(listenfd);
        exit(1);
    }

    if(listen(listenfd, 5) < 0){
        perror("listen failed");
        close(listenfd);
        exit(1);
    }
    printf("Server listening on port %d\n", PORTNO);
    while(1){
        socklen_t clilen = sizeof(cliaddr);
        if((connectfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen)) < 0){
            perror("accept failed");
            continue;
        }
        printf("Client connected\n");
        while(1){
            memset(buf, 0, MAX);  
            int n;
            if((n = recv(connectfd, buf, MAX-1, 0)) > 0){
                buf[n] = '\0';  
                printf("Message from client: %s", buf);
                
                if(strncmp(buf, "end", 3) == 0){
                    printf("Terminating client.\n");
                    break;
                }
                send(connectfd, buf, n, 0);  
            }
        }
        close(connectfd);
    }
    close(listenfd);
    return 0;
}