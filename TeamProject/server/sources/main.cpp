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

void thread_client(int socket){
			char buffer[1024];
			while(1) {
				recv( socket , buffer, 1024,0);

				//memset(buffer, 0, sizeof buffer);
				header* headerRequest = (header*) buffer;

				if(headerRequest->msgId == 1){
					code codeResponse;

					codeResponse.codeId = 200;
					send(socket,&codeResponse,sizeof(code),0);

					recv( socket , buffer, 1024,0);
					registration* registrationRequest = (registration*) buffer;

					//zapis do pliku
					std::ofstream fileUsers;
					fileUsers.open("../users.txt", std::ios_base::app);
					fileUsers<<registrationRequest->username<<":"<<registrationRequest->password<<'\n';
					fileUsers.close();

					codeResponse.codeId = 200;
					send(socket,&codeResponse,sizeof(code),0);

				}
				else if(headerRequest->msgId == 2){

				}
				else {
					close(socket);
					break;
				}

			}

}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

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
    if (listen(server_fd, 9999) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

while(1){

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
		std::cout << new_socket << '\n';

		std::thread t(thread_client,new_socket);
		t.detach();
    //send(new_socket , hello , strlen(hello) , 0 );
    //printf("disconnected\n");

	}
    return 0;
}

