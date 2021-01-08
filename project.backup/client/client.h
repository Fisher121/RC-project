#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <map>
#include <iostream>
#include <vector>
#include <sys/sendfile.h>
#include <string>
#include "../structs.h"

extern errno;

class client
{
	private:
	
		int sd;
		struct sockaddr_in server;
		char username[20];
		char *ipv4;
		int port;
	public:
		bool state = true;
		client(){port = 2024;ipv4 = "127.0.0.1";}
		~client();
		int Connect();
		void login(char* username, char* password);
		int init();
		void loop();
		void getCmd();
		void Close();
		void makeCmd(char *cmd,int sz, ...);
		void signIn(char* username, char* password, char* email);
		void getBookDetails(int bookID, book &bc);
		void getBook(int bookID,std::vector<str> &Vector);
		void searchBooks(search crit);
		void rate(int id,int rate);
		int downloadBook(int id);
		template<typename T>int Write(int sd,T msg);
        	template<typename T> int Read(int sd,T &message);
        	template<typename T> int writeVector(int sd,std::vector<T> Vector);
	        template<typename T> int readVector(int sd,std::vector<T> &Vector);

};



































