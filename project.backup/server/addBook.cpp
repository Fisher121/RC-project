#include "sqlite3.h"
#include <iostream>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../structs.h"
using namespace std;

int Callback(void *count,int argc,char **data,char **column)
{
	int *c = (int*)count;
        *c = atoi(data[0]);
        return 0;
}
std::string itoa(int a ,int c)
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

int main()
{
	sqlite3* db;
	int res = sqlite3_open("projectDatabase.db", &db);
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
	int ok = 1;
	do{
		char t[200],n[200];
		bzero(t,200);
		bzero(n,200);
		string id,an,ISBN,titlu,numeAutor,gen,subgen;
		printf("Book Name: ");
		fgets(t,200,stdin);
		t[strlen(t)-1] = '\0';
		titlu = t;
		printf("Autor: ");
		fgets(n,200,stdin);
		n[strlen(n)-1] = '\0';
		numeAutor = n;
		printf("Genuri: ");
		cin>>gen;
		printf("Subgenuri: ");
		cin>>subgen;
		printf("An: ");
		cin>>an;
		printf("ISBN: ");
		cin>>ISBN;
		
		std::string sql = "SELECT COUNT(*) FROM carti;";
		int k;
		char* errmsg = 0;
	        res = sqlite3_exec(db,sql.c_str(),Callback,&k,&errmsg);
		id = itoa(k,10);
		string ins ;
		
		ins = "INSERT INTO carti(titlu,bookID,an,ISBN,rating,ratingNum) VALUES ('" + titlu + "' , " + id + " , " + an + " , " + ISBN + " , 0 , 0 );";
		cout<<ins<<endl;
		res = sqlite3_exec(db,ins.c_str(),0,0,&errmsg);
		if(res != SQLITE_OK)
			printf("eroare: %s\n",errmsg);
	
		char g[200];
		strcpy(g,gen.c_str());	
		char* pch = strtok(g," ,");
		do{
		gen = pch;
		ins = "INSERT INTO genuri(bookID,gen) VALUES (" + id + " ,'" + gen + "');";
                cout<<ins<<endl;
                res = sqlite3_exec(db,ins.c_str(),0,0,&errmsg);
                if(res != SQLITE_OK)
                        printf("eroare: %s\n",errmsg);
		pch = strtok(NULL," ,");
		}
		while(pch !=NULL);


		strcpy(g,subgen.c_str());
                pch = strtok(g," ,");
                do{
                subgen = pch;
                ins = "INSERT INTO subgenuri(bookID,subgen) VALUES (" + id + " ,'" + subgen + "');";
                cout<<ins<<endl;
                res = sqlite3_exec(db,ins.c_str(),0,0,&errmsg);
                if(res != SQLITE_OK)
                        printf("eroare: %s\n",errmsg);
                pch = strtok(NULL," ,");
                }
                while(pch !=NULL);


		ins = "SELECT COUNT(*) FROM autori WHERE nume = '" + numeAutor +"';";
                res = sqlite3_exec(db,ins.c_str(),Callback,&k,&errmsg);
		if(res != SQLITE_OK)
                        printf("eroare: %s\n",errmsg);
		string idC;
		if(k == 0)
		{
			ins = "SELECT COUNT(*) FROM autori;";
			res = sqlite3_exec(db,ins.c_str(),Callback,&k,&errmsg);
        	        if(res != SQLITE_OK)
	                        printf("eroare: %s\n",errmsg);
			idC = itoa(k,10);
			ins = "INSERT INTO autori VALUES('" + numeAutor + "' , " + idC + ");";
			cout<<ins<<endl;
			res = sqlite3_exec(db,ins.c_str(),0,0,&errmsg);
               		if(res != SQLITE_OK)
                        	 printf("eroare: %s\n",errmsg);

		}
		else
		{
			ins = "SELECT autorID FROM autori WHERE nume = '" + numeAutor +"';";
			res = sqlite3_exec(db,ins.c_str(),Callback,&k,&errmsg);
                        if(res != SQLITE_OK)
                                printf("eroare: %s\n",errmsg);
			idC = itoa(k,10);

		}
		 ins = "INSERT INTO autoriCarti VALUES(" + id + " , " + idC + ");";
                        cout<<ins<<endl;
                        res = sqlite3_exec(db,ins.c_str(),0,0,&errmsg);
                        if(res != SQLITE_OK)
                                 printf("eroare: %s\n",errmsg);


		printf("Continuati?\n");
		cin>>ok;
	}while(ok);
	sqlite3_close(db);
}
