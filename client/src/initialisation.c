#include <client.h>



void initialisation(struct sockaddr_in *SERVER, int *sock, bool *connected)
{
	char _buff[BUFFER];
	short int _port;

	printf("\n\033[32mStarting client...\033[0m\n");
	sleep(1);

	printf("\nServer IP: ");
	scanf("%s", _buff);
	getchar();

	do
	{
		printf("Port: ");
		scanf("%hd", &_port);

	} while(_port < 1024);

	SERVER->sin_family      = AF_INET;
	SERVER->sin_port        = htons(_port);
	SERVER->sin_addr.s_addr = inet_addr(_buff);

	*sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	int result;
	short int _i;
	do
	{
		result = connect(*sock, (struct sockaddr*)SERVER, sizeof(*SERVER));

		if(result == -1)
		{
			printf("\n\n\033[31mError while connecting to %s:%hd.\nDo you want retry ? (yes / no)\033[0m\n:| ", inet_ntoa(SERVER->sin_addr), _port);
			scanf("%s", _buff);
			getchar();

			for(_i = 0; _i < (short int)strlen(_buff); _i++) { _buff[_i] = tolower(_buff[_i]); }

			if(!strcmp(_buff, "no") || !strcmp(_buff, "n"))
			{
				*connected = false;
				return;
			}
		}

		else
		{
			*connected = true;
			printf("\nConnected to %s:%hd.\n\n", inet_ntoa(SERVER->sin_addr), ntohs(SERVER->sin_port));
		}

	} while(result == -1 && (!strcmp(_buff, "yes") || !strcmp(_buff, "y")));
}


void disconnection(int sock)
{
	if(close(sock) == -1)
	{
		printf("\n\033[35mSocket couldn't be successfully closed.\033[0m\n");
	}

	printf("\n\n\033[35mSocket successfully closed.\033[0m\n\n");
	sleep(1);
	printf("WIFFS Client is shutting down for now !\n\n\n\n");
	sleep(1);
}
