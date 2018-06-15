// Server side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <fstream>
#include <mutex>
#include <vector>

#define PORT 8080

std::mutex plik;
std::mutex vect;
std::vector<int> usersOnline;
// msgId:
// 1:register
// 2:login
// 3:exit
// 
struct msg_header{
	unsigned int msgId;
};

struct msg{
  char name[20];
  char content[255];
};
struct code{
  int codeId;
};

struct msg_auth{
  msg_header header;
  char username[20];
  char password[20];
};
int existUsername(const char* curentUsername){
	std::ifstream fileUsers;
	fileUsers.open("../users.txt", std::ios_base::in);
	std::string line = "";
	while(!fileUsers.eof()){
		getline(fileUsers,line);
		char* newline = strdup(line.c_str());
		char* usern = strtok(newline,":");
		if (usern != NULL){
			if (strcmp(curentUsername,usern)==0) {
				fileUsers.close();
				return 1;
			}
		}
	}
	fileUsers.close();
	return 0;
}
void thread_client(int socket){
		printf("theard_client %d\n",socket);
		char buffer[1024];
		char nameUser[20]="";
		int error,logged;
		while(!error){
			while(!logged) {
				recv( socket , buffer, 1024,0);
				msg_auth* authRequest = (msg_auth*) buffer;
				code codeResponse;
				printf("I work\n");
				if(authRequest->header.msgId == 1){
					printf("try to register\n");
					std::lock_guard<std::mutex> lock(plik);
					if(!existUsername(authRequest->username)){
					//zapis do pliku
						std::ofstream fileUsers;
						fileUsers.open("../users.txt", std::ios_base::app);
						fileUsers<<authRequest->username<<":"<<authRequest->password<<'\n';
						fileUsers.close();
						codeResponse.codeId = 200;
						send(socket,&codeResponse,sizeof(code),0);
						logged = 1;
						usersOnline.push_back(socket);
						printf("Add sock %d to vector",socket);
						strcpy(nameUser, authRequest->username);
					}else{
						codeResponse.codeId = 201;
						send(socket,&codeResponse,sizeof(code),0);
					}

				}
				else if(authRequest->header.msgId == 2){
					printf("try to login\n");
					std::lock_guard<std::mutex> lock(plik);
					//odczyt z pliku
					std::ifstream fileUsers;
					fileUsers.open("../users.txt", std::ios_base::in);
					std::string line = "";
					while(!fileUsers.eof()){
						getline(fileUsers,line);
						char* newline = strdup(line.c_str());
						char* usern = strtok(newline,":");
						char* pwd = strtok(NULL,":");
						if (usern != NULL && pwd != NULL){
							if (!strcmp(usern,authRequest->username) && !strcmp(pwd,authRequest->password)){ 
							logged = 1;
							break;
							}
						}
					}
					fileUsers.close();

					if (logged){
						strcpy(nameUser, authRequest->username);
						codeResponse.codeId = 200;
						usersOnline.push_back(socket);
						printf("Add sock %d to vector\n",socket);
						send(socket,&codeResponse,sizeof(code),0);
						
					}
					else  codeResponse.codeId = 205;
					send(socket,&codeResponse,sizeof(code),0);
				}
				else {
					printf("\nWe have a problem =(\n");
					printf("\nauthRequest->header.msgId !=(1,2)\n");
					error = 1;
					
					break;
				}

			}
			if (logged!=1) { 
				error = 1; break;
			}
			printf("I connected user:%s \n",nameUser); 
			while(recv( socket , buffer, 1024,0)>0){
				printf("sent message socket: %d\n",socket);
				msg* message_n =(msg*) buffer;
				msg message = *message_n;
				std::lock_guard<std::mutex> lock(vect);
				for(int &sock : usersOnline){
					if(sock !=socket) {
						send(sock, &message, sizeof(msg),0);
						printf("sent message to %d\n", sock);
					}				
				}			
			}
			
		}
close(socket);
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

		//tworzenie pliku
		std::ifstream ifile("../users.txt");
		if(ifile.good() == false) {
			std::ofstream fileUsers("../users.txt");
			fileUsers.close();
		}

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    printf("\n=>Socket server has been created...\n");

    memset(&address, '0', sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 200) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while(1){
   	 if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
  	  {
   	     perror("accept error");
  	      exit(EXIT_FAILURE);
  	  }
	  printf("=>Connected new client :socket_id = %d\n",new_socket);
	  std::thread t(thread_client,new_socket);
	  t.detach();

}
    return 0;
}

