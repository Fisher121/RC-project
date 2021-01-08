#pragma once
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include "sqlite3.h"
#include <csignal>
#include "../structs.h"


static void handler(int sig)
{
        pid_t pid;
        while(1)
        {
                pid=waitpid(-1,NULL, WNOHANG);
                if(pid == 0)
                        return;
                else
                        if(pid == -1)
                                return;
        }
}
static std::string itoa(int a ,int c)
{
        int cpy=a,count = 0;
        char z[10];
        std::string id;
        bzero(z,10);
        if(a == 0)
                id = '0';
        else
        {
        while(a!=0)
        {
                count ++;
                a=a/10;
        }
        while(count-1>=0)
        {
                z[count-1] = '0' + cpy%10;
                cpy = cpy/10;
                count--;
        }
        id = z;
        }
        return id;
}

class Server{
private:
	int PORT;
	struct sockaddr_in server;
	struct sockaddr_in from;
	int sd,pid,res;
	sqlite3* db;
	int client;
public:
	Server(){ PORT = 2024;}
	~Server();
	int init();
	int serveClients();
	void loop();
	bool login(char* username, char* password);
	int signIn(char* username, char* password, char *email);
	int sendBook(int bookID);
	int getBooks(std::vector<book> &bookVector,search src);
	int getBookDetails(int id, book &b);
	int Rate(int id,int rate,str user);
	int addAcc(str user,int id);

	int sendFile(int id);
	template<typename T>int Write(int sd,T msg);
      	template<typename T> int Read(int sd,T &message);
       	template<typename T> int writeVector(int sd,std::vector<T> Vector);
       	template<typename T> int readVector(int sd,std::vector<T> &Vector);


};
