#include "client.h"

int client::init()
{
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ipv4);
	server.sin_port = htons(port);
	return 1;
}

int client::Connect()
{
	if((sd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
        	perror("[client]Eroare la socket.\n");
       		 return errno;
	}

	if(connect(sd, (struct sockaddr*)&server, sizeof(struct sockaddr)) == -1)
	{
        	perror("[client]Eroare la connect().\n");
                return errno;
	}
	else
		return 1;
}

template<typename T>int client::Write(int sd,T msg)
{
        int size = sizeof(T);
        if(write(sd,&size, sizeof(int)) <= 0)
        {
                perror("[server]Eroare la write() catre server(1).\n");
                return errno;
        }
        if(write(sd, &msg,size) <= 0)
        {
                perror("[server]Eroare la write() catre server(2).\n");
                return errno;
        }
        return true;
}

template<typename T>int client::Read(int sd,T &message)
{
	int index;
	if(read(sd,&index,sizeof(index)) < 0)
	{
       		perror("[client]Error on read() from server(1).\n");
        	return errno;
	}
	if(read(sd,&message,index) < 0)	
	{
        	perror("[client]Error on read() from server(2).\n");
        	return errno;
	}
	return 1;
}
template<typename T>int client::writeVector(int sd,std::vector<T> Vector)
{
        int sz = Vector.size();
        this->Write<int>(sd,sz);
        for(int i = 0;i<=Vector.size()-1;i++)
        {
                if(write(sd,&Vector[i],sizeof(T)) <= 0)
	        {
                	perror("[server]Eroare la write() catre server(2).\n");
                	return errno;
       		}

        }
        return 1;
}
template<typename T>int client::readVector(int sd,std::vector<T> &Vector)
{
        int sz;
	T buffer;
        this->Read<int>(sd,sz);
        for(int i = 0;i<=sz-1;i++)
        {
		if(read(sd,&buffer,sizeof(T)) < 0)
       		{
                	perror("[client]Error on read() from server.\n");
                	return errno;
	        }
		else
		{

			Vector.push_back(buffer);
		}
        }
        return 1;
}

int client::downloadBook(int id)
{
	book b;
	int sz;
	std::string s = "books/",titlu;
	this->getBookDetails(id,b);
	titlu = b.titlu;
	s = s + titlu;
	FILE * f = fopen(s.c_str(),"w+");
	int fd = open(s.c_str(),O_WRONLY | O_RDONLY);
	if(fd == 0)
	{
		perror("[client]Eroare la creare fisier!\n");
		return errno;
	}
	this->Connect();
	str usr;
	strcpy(usr.buffer,this->username);
	this->Write<int>(sd,7);
	this->Write<int>(sd,id);
	this->Write<str>(sd,usr);
	FILE *fp;
	int bytesReceived;
	char recvBuff[256];
    	fp = fopen(s.c_str(), "ab");
    	if(NULL == fp)
    	{
        	printf("Error opening file");
        	return 1;
    	}

    	while((bytesReceived = read(sd, recvBuff, 256)) > 0)
    	{
        	fwrite(recvBuff, 1,bytesReceived,fp);
   	}

    	if(bytesReceived < 0)
    	{
        	printf("\n Read Error \n");
    	}
	return 1;
}


void client::getCmd()
{
	int cmd;
	char username[20],password[20],email[320];
	printf("\n1.login\n");
	printf("2.sign in\n");
	printf("3.Cautati cartea dupa nume\n");
	printf("4.Cititi carte\n");
	printf("5.Detalli carte\n");
	printf("6.Download a book\n");
	printf("7.Recommend book\n");
	printf("9.Quit\n");
	scanf("%d",&cmd);
	switch (cmd){
		case 1:
			printf("username: ");
			scanf("%s",username);
			printf("password: ");
			scanf("%s",password);
			this->login(username,password);
			break;
		case 2:
                        printf("username: ");
                        scanf("%s",username);
                        printf("password: ");
                        scanf("%s",password);
			printf("email: ");
			scanf("%s",email);
                        this->signIn(username,password,email);
			break;
		case 3:
			{
			search crit;
			bzero(crit.an,5);
			bzero(crit.ISBN,14);
			bzero(crit.rating,3);
			bzero(crit.autor,200);
			bzero(crit.titlu,200);
			bzero(crit.gen,200);
			printf("Introduceti 0 daca doriti sa sariti vre-un pas!\n");
			printf("Nume carte: ");
			bzero(crit.titlu,200);
			fgets(crit.titlu,200,stdin);
			printf("Autor : ");
			bzero(crit.autor,200);
			fgets(crit.autor,200,stdin);
			printf("Genuri(separate prin '-')");
			std::cin>>crit.gen;
			printf("Subgenuri(separate prin '-')");
			std::cin>>crit.subgen;
			printf("ISBN: ");
			std::cin>>crit.ISBN;
			printf("an aparitie: ");
			std::cin>>crit.an;
			printf("rating: ");
			std::cin>>crit.rating;
			this->searchBooks(crit);
			break;
			}
		case 4:
			{
			std::vector<str> Vector;
			int bookID;
			printf("bookID:");
			std::cin>>bookID;
			this->getBook(bookID,Vector);
			for(int i=0;i<=Vector.size()-1;i++)
			{
				printf("%s",Vector[i].buffer);
			}
			printf("\n\n\n\nVrei sa dai o nota cartii intre 0-5?(0 = NU)");
			int z;
			std::cin>>z;
			if(z >0 && z<=5)
				this->rate(bookID,z);
			else
				this->rate(bookID,0);

			}
			break;
		case 5:
			{
			int id;
			printf("ID-ul cartii: ");
			std::cin>>id;
			book b;
			this->getBookDetails(id,b);
			printf("\ntitlu: %s\n",b.titlu);
        		printf("autor: %s\n",b.numeAutor);
        		printf("genuri: %s\n",b.gen);
        		printf("subgenuri: %s\n",b.subgen);
      			 printf("an aparitie: %d\n",b.an);
       			 printf("ISBN: %ld\n",b.ISBN);
       			 printf("rating: %f\n",b.rating);
			}
			break;
		case 6:
			{
				int id;
				printf("id-ul cartii dorite: ");
				std::cin>>id;
				this->downloadBook(id);
			}
			break;	
		case 9:
			state = false;
			break;

	}
}


void client::getBook(int bookID, std::vector<str> &Vector)
{
	this->Connect();
	this->Write(sd,4);
	this->Write<int>(sd,bookID);
	this->readVector<str>(sd,Vector);
	this->Close();
}
void client::login(char *username, char *password)
{
	str toSend;
	int msg;
	this->Connect();
        Write<int>(sd,1);
	this->makeCmd(toSend.buffer,2,username,password);
	this->Write<str>(sd,toSend);
	this->Read<int>(sd,msg);

	if(msg == 1)
	{
		printf("V-ati logat cu succes!\n");
		strcpy(this->username,username);
	}
	else
	{
		printf("Username sau parola necunoscuta!\n");
	}
	this->Close();
}

void client::signIn(char* username,char* password, char* email)
{
	str toSend;
	int msg;
	this->Connect();
        Write<int>(sd,2);
	this->makeCmd(toSend.buffer,3,username,password,email);
	this->Write<str>(sd,toSend);
	this->Read<int>(sd,msg);
	if(msg == 1)
	{
		printf("Cont creat cu succes!\n");
	}
	else
	{
		printf("Contul nu a putut fi creat!\n");
	}
	this->Close();

}
void client::Close()
{
	close(sd);
}

void client::makeCmd(char* cmd, int sz, ...)
{
	char c[4096];
	va_list args;
	va_start(args,sz);
	bzero(c,4096);
	strcat(c,va_arg(args,char*));
	c[strlen(c)] = ',';
	for(int i = 2 ;i<=sz;i++)
	{
		strcat(c,va_arg(args,char*));
		c[strlen(c)] = ',';
	}
	va_end(args);
	strcpy(cmd,c);

}



void client::searchBooks(search crit)
{
	str cmd;
	this->Connect();
        Write<int>(sd,3);
	this->makeCmd(cmd.buffer,6, crit.titlu,crit.autor,crit.gen,crit.an,crit.ISBN,crit.rating);
 	this->Write<str>(sd,cmd);
       	std::vector<book> rasp;
       	this->readVector<book>(sd,rasp);
	std::cout<<"Cartile gasite sunt: \n";
	for(auto c : rasp)
		printf("titlu: %s        ID: %d\n",c.titlu,c.ID);
	this->Close();
      	

}

void client::getBookDetails(int bookID,book &bc)
{
	book b;
	this->Connect();
	this->Write<int>(sd,5);
	this->Write<int>(sd,bookID);
	this->Read<book>(sd,b);
	bc = b;
	this->Close();
}	

void client::rate(int id,int rate)
{
	str s;
       	bzero(s.buffer,4096);
       	strcpy(s.buffer,username);

	this->Connect();
	this->Write<int>(sd,6);
	this->Write<int>(sd,id);
	this->Write<int>(sd,rate);
	this->Write<str>(sd,s);
	this->Close();
}






