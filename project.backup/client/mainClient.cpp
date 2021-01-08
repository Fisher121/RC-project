#include "client.h"
#include "interface.h"
int main()
{
	client *Client = new client;
	Client->init();
	do
	{
		Client->getCmd();
	}while(Client->state);
}
