#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdbool.h>
#define MAX 4096
#define PORT 8080
#define SA struct sockaddr

/* function prototype */
void func(int sockfd);
void bzero(void *s, size_t n);

/*

Template of the server downloaded from https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/

*/

/* Function designed for chat between client and server. */
void func(int sockfd)
{
	char buff[MAX];
	int idx, i, j;
	bool match;

	/* infinite loop for chat*/
	for (;;) {
		bzero(buff, MAX);
		idx = -1;
		match = false;
		/* read the message from client and copy it in buffer */
		read(sockfd, buff, sizeof(buff));

        /* if msg contains "Exit" then server exit and chat ended. */
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
		}
		/*
		  do we hava a space in the buffer ?
		*/
		for(i=0; i<MAX; i++)
		 if(buff[i] ==' ')
		  {
 		   idx = i;
		   break;
		  }
        if(idx >0)
		for(i = idx; i<MAX-idx; i++) /* searching for a substring */
		 {
 		  match = true;
 		  for(j=0; j < idx; j++)
 		   if(buff[j]!=buff[i+j])
		    {
		     match = false;
		     break;
		    }
		 if(match==true)
          {
		    idx = i-idx;
		    break;
		   }
		 }
		/*clear the buffer */
	    bzero(buff, MAX);

		if(match && idx!=-1)
		  snprintf(buff, MAX, "%d\n", idx-1);else
          snprintf(buff, MAX, "%d\n", -1);

        printf(buff, sizeof(buff));
        /* and send that buffer to client */
        write(sockfd, buff, sizeof(buff));
	}
}

int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;


	/* socket create and verification */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	bzero(&servaddr, sizeof(servaddr));

	/* assign IP, PORT */
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	/* Binding newly created socket to given IP and verification */
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}

	/* Now server is ready to listen and verification */
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}

	len = sizeof(cli);

	/* Accept the data packet from client and verification */
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server acccept failed...\n");
		exit(0);
	}

	/* Function for chatting between client and server */
	func(connfd);

	/* After chatting close the socket */
	close(sockfd);
}

