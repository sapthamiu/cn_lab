//Write an iterative TCP client-server program where the client accepts a sentence from the user and sends it to the server. 
//The server will check for duplicate words in the string. Server will find number of occurrences of duplicate words present 
//and remove the duplicate words by retaining single occurrence of the word and send the resultant sentence to the client. 
//The client displays the received data on the client screen. 
//The process repeats until the user enters the string “Stop”. Then both the processes terminate.
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define PORTNO 14201
#define MAX 100
int wordcount;
char words[MAX][MAX];
int isDupl(char* word){
    for(int i = 0; i < wordcount; i++){
        if(strcmp(word, words[i]) == 0)
            return 1;
    }
    return 0;
}
void remDupl(char* str, char* result){
    wordcount = 0; 
    char temp[100];
    strcpy(temp, str); 
    
    char* token = strtok(temp, " ");
    result[0] = '\0'; 
    
    while(token != NULL){
        if(!isDupl(token)){
            strcpy(words[wordcount], token);
            wordcount++;
            if(strlen(result) > 0)
                strcat(result, " ");
            strcat(result, token);
        }
        token = strtok(NULL, " ");
    }
}
int main(){
    char str[MAX], result[MAX];;
    struct sockaddr_in seraddr, cliaddr;
    int listenfd, connectfd, n;
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket creation failed");
        exit(1);
    }
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(PORTNO);
    seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(bind(listenfd, (struct sockaddr*)&seraddr, sizeof(seraddr)) < 0){
        close(listenfd);
        perror("bind failed");
        exit(1);
    }
    listen(listenfd, 5);
    printf("server listening on port %d\n", PORTNO);
    while(1){
        socklen_t clilen = sizeof(cliaddr);
        if((connectfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen)) < 0){
            perror("accept failed");
            continue;
        }
        while(1){
            memset(str, 0, MAX);
            if((n = recv(connectfd, str, sizeof(str), 0)) > 0){
                str[n] = '\0';
                printf("Received: %s\n", str);
                
                if(strcmp(str, "Stop") == 0){
                    printf("Client requested termination\n");
                    close(connectfd);
                    break;
                }
                
                memset(result, 0, MAX);
                remDupl(str, result);
                send(connectfd, result, strlen(result), 0);
                printf("Sent: %s\n", result);
            }
        }
        close(connectfd);

    }
    close(listenfd);
    return 0;
}