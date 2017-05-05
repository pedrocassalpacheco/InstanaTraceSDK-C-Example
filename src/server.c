#include "server.h"
#include "http.h"
#include "instana.h"


#define MY_PORT         9999
#define MAXBUF          4094
#define CNT sizeof(urls)/sizeof(char *)

int gSockfd;

void signal_callback_handler(int signum)
{
	printf("\n**** Cntr-C detected. ****\n");

	close(gSockfd);
	exit(signum);
}

void memory_profile()
{
		struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);

		/*printf("\nmaximum resident set size:%d",ru.ru_maxrss);
		printf("\nintegral shared memory size:%d",ru.ru_ixrss);
		printf("\nintegral unshared data size:%d",ru.ru_idrss);
		printf("\nintegral unshared stack size:%d",ru.ru_isrss);
		printf("\npage reclaims (soft page faults):%d",ru.ru_minflt);
		printf("\npage faults (hard page faults):%d",ru.ru_majflt);
		printf("\nswaps:%d",ru.ru_nswap);
		printf("\nblock input operations:%d",ru.ru_inblock);
		printf("\nblock output operations:%d",ru.ru_oublock);
		printf("\nIPC messages sent:%d",ru.ru_msgsnd);
		printf("\n IPC messages received:%d",ru.ru_msgrcv);
		printf("\nsignals received:%d",ru.ru_nsignals);
		printf("\nvoluntary context switches:%d",ru.ru_nvcsw);
		printf("\ninvoluntary context switches:%d",ru.ru_nivcsw);*/
}

int main(int Count, char *Strings[])
{

    	struct sockaddr_in self;
    	char buffer[MAXBUF];

			// Handles kill and cntr c
    	signal(SIGINT, signal_callback_handler);

    	// Create streaming socket
    	if ( (gSockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    	{
        	perror("Socket");
        	exit(errno);
    	}

    	// Initialize address/port structure
    	bzero(&self, sizeof(self));
    	self.sin_family      = AF_INET;
    	self.sin_port        = htons(MY_PORT);
    	self.sin_addr.s_addr = INADDR_ANY;

    	// Assign a port number to the socket
    	if ( bind(gSockfd, (struct sockaddr*)&self, sizeof(self)) != 0 )
    	{
        	perror("socket--bind");
        	exit(errno);
    	}

    	// Make it a "listening socket
    	if ( listen(gSockfd, 20) != 0 )
    	{
        	perror("socket--listen");
        	exit(errno);
    	}


        for (;;)
        {
            int clientfd;
            struct sockaddr_in client_addr;
            int addrlen = sizeof(client_addr);

            // Accept a connection (creating a data pipe)
            clientfd = accept(gSockfd, (struct sockaddr*)&client_addr, &addrlen);


						printf("\n**** %s:%d connected ****\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            // Payload for HTTP
						kv data[2];
						strcpy(data[0].key,"IP Address");
						strcpy(data[0].value, inet_ntoa(client_addr.sin_addr));
						strcpy(data[1].key,"Port");
						sprintf(data[1].value, "%d",ntohs(client_addr.sin_port));

						entry_point("Custom HTTP Server", data);
            int bytesIn = recv(clientfd, buffer, MAXBUF,0);

            // Memory profile
						intermidiate_point("Memory Profiling", NULL);
						get("https://sesandbox-instana.instana.io");
            memory_profile();

            // Echo back anything sent
            send(clientfd, buffer, bytesIn, 0);

						// Data to be sent to Instana
						kv response[1];
						strcpy(response[0].key ,"Response");
						strcpy(response[0].value, "HTTP 400");

						// Exit point
						exit_point("Response", response);

            // Close data connection
            close(clientfd);
        }

        // All cleanup done by signal handler

    return 0;
}
