// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 8080
  
struct header{
	int msgId;
};


struct code{
  int codeId;
};

struct registration{
  char username[20];
  char password[20];
};

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    int bufsize = 1024;
    char buffer[bufsize];
    header operation;
    code* codeResponse;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    printf("\n=>Socket client has been created..\n");

    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    printf("\n=>Connection to the server port\n");
    do{
    printf("Registration:1\nLogin:2\n");
    scanf("%d",&operation.msgId);
    send(sock, &operation.msgId,sizeof(header),0);
    recv( sock , buffer, 1024,0);
    codeResponse = (code*) buffer;
    }
    while(codeResponse->codeId!=200);
    registration user;
    printf("\nYour username:");
    scanf("%s",&user.username);
    printf("\nYour password:");
    scanf("%s",&user.password);
    send(sock, &user,sizeof(registration),0);
    recv( sock , buffer, 1024,0);
    codeResponse = (code*) buffer;
    if(codeResponse->codeId==200){
    printf("\nSuccess \n");
    }else printf("\nerror n/p \n");
    //send(sock , hello , strlen(hello) , 0 );
    //printf("Hello message sent\n");
    //valread = read( sock , buffer, 1024);
    //printf("%s\n",buffer );
    return 0;
}
