#include "serverClass.h"
extern int errno;

int Server::init()
{
res = sqlite3_open("projectDatabase.db", &db);
if(res != SQLITE_OK)
{
      perror("[server]Eroare la deschidere baza de date.\n");
      printf("%s\n",sqlite3_errmsg(db));
      sqlite3_close(db);
      return errno;
}
else
{
	printf("[server]Baza de data deschisa cu succes!\n");
}


	if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
{
        perror("[server]Eroare la socket().\n");
        return errno;
}

bzero(&server, sizeof(server));
bzero(&from, sizeof(from));

server.sin_family = AF_INET;
server.sin_addr.s_addr = htonl(INADDR_ANY);
server.sin_port = htons(PORT);

if(bind(sd,( struct sockaddr*)&server,sizeof(struct sockaddr)) == -1)
{
        perror("[server]Eroare la bind.\n");
        return errno;
}

if(listen(sd,5) == -1)
{
        perror("[server]Eroare la listen.\n");
        return errno;
}
return 1;
}

template<typename T>int Server::Write(int sd,T msg)
{
        int size = sizeof(T);
        if(write(sd,&size, sizeof(int)) <= 0)
        {
                perror("[server]Eroare la write() catre client(1).\n");
                return errno;
        }
        if(write(sd, &msg,size) <= 0)
        {
                perror("[server]Eroare la write() catre client(2).\n");
                return errno;
        }
        return true;
}

template<typename T>int Server::Read(int sd,T &message)
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

template<typename T>int Server::writeVector(int sd,std::vector<T> Vector)
{
        int sz = Vector.size();
        this->Write<int>(sd,sz);
        for(int i = 0;i<=Vector.size()-1;i++)
        {
                if(write(sd,&Vector[i],sizeof(T)) <= 0)
                {
                        perror("[server]Eroare la write() catre server.\n");
                        return errno;
                }
        }
        return 1;

}
template<typename T>int Server::readVector(int sd,std::vector<T> &Vector)
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

int Server::sendFile(int id)
{	
	std::string i = itoa(id,10);
	std::string s = "books/";
	s = s + i;
	FILE *fp = fopen(s.c_str(),"rb");
        if(fp==NULL)
        {
            printf("File open error");
            return 1;   
        }   

        while(1)
        {
            unsigned char buff[256]={0};
            int nread = fread(buff,1,256,fp);
            if(nread > 0)
            {
                write(client, buff, nread);
            }
	    if(nread<256)
	    {
                if (ferror(fp))
                    printf("Error reading\n");
		return 1;
	    }
        }
	return 1;
}

int Server::serveClients()
{
        socklen_t length = sizeof(from);
        int size;

	signal(SIGCHLD,handler);
	printf("[server]Asteptam la portul %d...\n",PORT);
        fflush(stdout);

	client = accept(sd, (struct sockaddr*) &from, &length);
        if(client < 0 )
        {
                perror ("[server]Eroare la accept.\n");
		close(client);
		return errno;
        }


        if((pid = fork()) < 0)
        {
                perror("[server]Eroare la fork().\n");
                close(client);
        }

	if(pid == 0)
        {
		int cmdID;
		this->Read<int>(client,cmdID);
		switch (cmdID) {
			case 1:
				{
				str msg;
				this->Read<str>(client,msg);
				char* pch;
				char username[20],password[20];	
				int raspuns;
				pch = strtok(msg.buffer,", ");
				strcpy(username,pch);
				pch = strtok(NULL,", ");
				strcpy(password,pch);
				raspuns = login(username,password);
				printf("[server]Trimitem mesajul inapoi...%d\n",raspuns);
               			this->Write<int>(client,raspuns);
				}
				break;
			case 2:
				{
				char *pch;
				char username[20],password[20],email[320];
				int raspuns;
				str msg;
				this->Read<str>(client,msg);
				pch = strtok(msg.buffer,", ");
				strcpy(username,pch);
				pch = strtok(NULL,", ");
				strcpy(password,pch);
				pch = strtok(NULL,", ");
				strcpy(email,pch);
				raspuns = signIn(username,password,email);
				printf("[server]Trimitem mesajul inapoi...%d\n",raspuns);
        		        this->Write<int>(client,raspuns);
	
				}
				break;
			case 3:
				{
				str msg;
				char* pch;
				this->Read<str>(client,msg);
				search src;
				std::vector<book> bookVector;
				pch = strtok(msg.buffer,", ");
				strcpy(src.titlu,pch);
				pch = strtok(NULL,", ");
                                strcpy(src.autor,pch);
				pch = strtok(NULL,", ");
                                strcpy(src.gen,pch);
				pch = strtok(NULL,", ");
                                strcpy(src.an,pch);
				pch = strtok(NULL,", ");
                                strcpy(src.ISBN,pch);
				pch = strtok(NULL,", ");
                                strcpy(src.rating,pch);
				this->getBooks(bookVector,src);
								}
				break;
			case 4:
				int bookID;
				this->Read<int>(client,bookID);
				this->sendBook(bookID);
					break;
			case 5:
				int id;
				book b;
				this->Read<int>(client,id);
				this->getBookDetails(id,b);
				this->Write<book>(client,b);
				break;
			case 6:
				{
					int id,rate;
					str user;
					this->Read<int>(client,id);
					this->Read<int>(client,rate);
					this->Read<str>(client,user);
					if(rate !=0)
						this->Rate(id,rate,user);
					this->addAcc(user,id);
				}
				break;
			case 7:
				{
				int id;
				this->Read<int>(client,id);
				this->sendFile(id);
				}
				break;
			default:
				str msg;
				strcpy(msg.buffer,"unknown_command");
				this->Write<str>(client,msg);
				break;

		}
		close(client);
                exit(1);
        }
        else
        {
                close(client);
		return 1;
        }
}
void Server::loop()
{
	int cond = 1;
	while(cond == 1)
	{
		cond = this->serveClients();
	}
}

int loginCallback(void *count,int argc,char **data,char **column)
{
        int *flag = (int*)count;
        *flag=1;
        return 0;
}

bool Server::login(char* username, char* password)
{
	int res;
	char* errmsg = 0;
        std::string us(username),pass(password);
        std::string sql = "SELECT * FROM users WHERE username='" + us + "' and password='" + pass + "';";
        int ok=0;
        res = sqlite3_exec(db,sql.c_str(),loginCallback,&ok,&errmsg);
        if(res != SQLITE_OK)
        {
                printf("[server]Eroare la login!\n%s\n",errmsg);
                return -1;
        }
        if(ok == 0)
        	return 0;
	return 1;
}

int Server::signIn(char* username, char* password, char*  email)
{
	int res;
	char* errmsg = 0;
	std::string us(username),pass(password),em(email);
	std::string sql = "SELECT * FROM users WHERE username='" + us + "';";
	int ok=0;
	res = sqlite3_exec(db,sql.c_str(),loginCallback,&ok,&errmsg);
	if(res != SQLITE_OK)
	{
		printf("[server]Eroare la adaugare user nou!(1)\n%s\n",errmsg);
		return -1;
	}
	else
	{
		if(ok != 0)
			return 0;
	}
	sql = "INSERT INTO users VALUES('" + us + "','" + pass + "','" + em + "');";
	res = sqlite3_exec(db,sql.c_str(),0,0,&errmsg);
	if(res == SQLITE_OK)
	{
		return 1;
	}	
	else
	{
		printf("[server]Eroare la adaugare user nou!\n%s\n",errmsg);
		return -1;
	}
}	

int SearchCallback(void *count,int argc,char **data,char **column)
{
	bv* flag = (bv*)count;
        book b;
	strcpy(b.titlu,data[0]);
	b.ID = atoi(data[1]);
	flag->b.push_back(b);
        return 0;
}

int Server::getBooks(std::vector<book> &bookVector,search src)
{
	book Book;
	char* errmsg = 0;
	std::string titlu,autor,gen,subgen,an,ISBN,rating,ins;
	titlu = src.titlu;
	autor = src.autor;
	an = src.an;
	ISBN = src.ISBN;
	rating = src.rating;
	int count = 0;
	bv boo;
	ins = "SELECT DISTINCT titlu, c.bookID FROM carti c JOIN autoriCarti ac ON c.bookID = ac.bookID JOIN autori a ON ac.autorID = a.autorID WHERE ";
	if(titlu[0] != '0')
	{
		count ++;
		ins = ins + "titlu LIKE '" + titlu + "%' ";
	}
	
	if(autor[0] !='0')
	{
		if(count != 0)
                	ins = ins + "AND ";

		count ++;
		ins = ins + "a.nume LIKE '" + src.autor + "%' ";
	}

	if(an[0] != '0' )
	{
		if(count != 0)
                	ins = ins + "AND ";

		count ++;
		ins = ins + "c.an = " + an + " ";
	}

	if (ISBN[0] !='0')
	{
		if(count == 0)
                	ins = ins + "AND ";
		count ++;
		ins = ins + "c.ISBN = " + ISBN + " ";
	}

	if(rating[0] !='0')
	{
		if(count != 0)
 	               ins = ins + "AND ";
		count ++;
		ins = ins + "c.rating/c.ratingNum >= " + rating + " ";
	}
	if(src.gen[0] != '0')
	{
		char* pch = strtok(src.gen,"- ");
		gen = pch;		
		if(count != 0)
                	ins = ins + "AND ";
		count ++;
		ins = ins + "c.bookID IN (SELECT bookID FROM genuri WHERE gen like '" + gen + "%') ";
		pch = strtok(NULL,"- ");
		while(pch != NULL)
		{
			
			ins = ins + "AND ";
			gen  = pch;
			std::cout<<"pch : " << pch<<std::endl;
			ins = ins + "c.bookID IN (SELECT bookID FROM genuri WHERE lower(gen) like '" + gen + "%') ";
			count ++;	
			pch = strtok(NULL, "- ");
		}
	
	}
	if(src.gen[0] != '0')
        {
                char* pch = strtok(src.subgen,"- ");
                subgen = pch;
                if(count != 0)
                        ins = ins + "AND ";
		count++;
                ins = ins + "c.bookID IN (SELECT bookID FROM subgenuri WHERE subgen like '" + subgen + "%') ";
                pch = strtok(NULL,"- ");
                while(pch != NULL)
                {

                        ins = ins + "AND ";
                        subgen  = pch;
                        ins = ins + "c.bookID IN (SELECT bookID FROM subgenuri WHERE lower(subgen) like '" + subgen + "%') ";
                        count ++;
                        pch = strtok(NULL, "- ");
                }

        }
	ins = ins + ';';
	printf("%s\n",ins.c_str());	
	res = sqlite3_exec(db,ins.c_str(),SearchCallback,&boo,&errmsg);
        if(res != SQLITE_OK)
        {
                printf("[server]Eroare la adaugare user nou!(1)\n%s\n%s\n",errmsg,ins.c_str());
                return -1;
        }

	bookVector = boo.b;
	this->writeVector<book>(client,bookVector);
	return 1;
}

int Server::sendBook(int bookID)
{
	int fd,cpy = 0;
	char path[20];
	std::string id = itoa(bookID,10);
	str buffer;
	std::vector<str> toSend;
	bzero(path,20);
	strcpy(path,"books/");
	strcat(path,id.c_str());
	if((fd = open(path, O_RDONLY)) <0)
	{
		printf("[server]Eroare la deschidere fisier!\n");
		strcpy(buffer.buffer,"ID necunoscut!\n");
		toSend.push_back(buffer);
		this->writeVector<str>(client,toSend);
			return errno;
	}
	bzero(buffer.buffer,4096);
	int x = 1;
	while(read(fd,buffer.buffer,sizeof(buffer)-1) > 0)
	{
		toSend.push_back(buffer);
		bzero(buffer.buffer,4096);
	}
	this->writeVector<str>(client,toSend);
	return 1;
}

int bookCallback(void *count,int argc,char **data,char **column)
{
        book* flag = (book*)count;
	book b;
        strcpy(b.titlu,data[0]);
        b.an = atoi(data[1]);
	b.ISBN = atoi(data[2]);
	float a = atoi(data[3]);
	float c = atoi(data[4]);
	if(c != 0 )
		b.rating = a/c;
	else
		b.rating = 0;
	*flag = b;
        return 0;
}

int newCallback(void *count,int argc,char **data,char **column)
{
	str* flag = (str*)count;
	if(strlen(flag->buffer) >0)
	{
		strcat(flag->buffer,",");
	}
	strcat(flag->buffer,data[0]);
	return 0;
}

int Server::getBookDetails(int id, book &b)
{
	char *errmsg = 0;
	std::string d = itoa(id,10);
	std::string ins = "SELECT titlu,an,ISBN,rating,ratingNum FROM carti WHERE bookID = " + d +";";
	res = sqlite3_exec(db,ins.c_str(),bookCallback,&b,&errmsg);
        if(res != SQLITE_OK)
        {
                printf("[server]Eroare la adaugare user nou!(1)\n%s\n%s\n",errmsg,ins.c_str());
                return -1;
        }
	str s;
	bzero(s.buffer,4096);
	ins = "SELECT gen FROM genuri WHERE bookID = " + d + " ;";
	res = sqlite3_exec(db,ins.c_str(),newCallback,&s,&errmsg);
        if(res != SQLITE_OK)
        {
                printf("[server]Eroare la adaugare user nou!(1)\n%s\n%s\n",errmsg,ins.c_str());
                return -1;
        }
	strcpy(b.gen,s.buffer);

	bzero(s.buffer,4096);
        ins = "SELECT subgen FROM subgenuri WHERE bookID = " + d + " ;";
        res = sqlite3_exec(db,ins.c_str(),newCallback,&s,&errmsg);
        if(res != SQLITE_OK)
        {
                printf("[server]Eroare la adaugare user nou!(1)\n%s\n%s\n",errmsg,ins.c_str());
                return -1;
        }
	strcpy(b.subgen,s.buffer);

	bzero(s.buffer,4096);
        ins = "SELECT nume FROM autori NATURAL JOIN autoriCarti WHERE bookID = " + d + " ;";
        res = sqlite3_exec(db,ins.c_str(),newCallback,&s,&errmsg);
        if(res != SQLITE_OK)
        {
                printf("[server]Eroare la adaugare user nou!(1)\n%s\n%s\n",errmsg,ins.c_str());
                return -1;
        }

	strcpy(b.numeAutor,s.buffer);
	return 1;
}


int Server::Rate(int id,int rate,str user)
{
	char* errmsg = 0;
	std::string ins;
	std::string bookID = itoa(id,10);
	std::string rating = itoa(rate,10);
	std::string username = user.buffer;
	ins = "UPDATE carti SET ratingNum = ratingNum+1 , rating = rating + " + rating + " WHERE bookID = " + bookID + " ;";
       res = sqlite3_exec(db,ins.c_str(),0,0,&errmsg);
       if(res != SQLITE_OK)
       {
                printf("[server]Eroare la adaugare user nou!(1)\n%s\n%s\n",errmsg,ins.c_str());
                return -1;
       }
       ins = "INSERT INTO userRating VALUES('" + username + "' , " + bookID + " , " + rating + ");";
       res = sqlite3_exec(db,ins.c_str(),0,0,&errmsg);
       if(res != SQLITE_OK)
       {
                printf("[server]Eroare la adaugare user nou!(1)\n%s\n%s\n",errmsg,ins.c_str());
                return -1;
       }
       return 1;
}


int Server::addAcc(str user,int id)
{
	char* errmsg = 0;
	std::string ins;
	std::string username = user.buffer;
	std::string bookID = itoa(id,10);
	ins = "INSERT INTO bookRead VALUES('" + username + "' , " + bookID + ");";
	res = sqlite3_exec(db,ins.c_str(),0,0,&errmsg);
       if(res != SQLITE_OK)
       {
                printf("[server]Eroare la adaugare user nou!(1)\n%s\n%s\n",errmsg,ins.c_str());
                return -1;
       }
       return 1;

}












