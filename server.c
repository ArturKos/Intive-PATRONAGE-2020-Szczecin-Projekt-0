#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#define MAX 4096
#define SA struct sockaddr
/* function prototype */

void bzero(void *s, size_t n);

/*

Template of the server downloaded from https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/

*/

int main(int argc, char* argv[])
{
	int sockfd, connfd;
	socklen_t len;
	struct sockaddr_in servaddr, cli;
	char localhost[] = "localhost\n";
    char buff[MAX];
	int idx, i, j;
	bool match;

    if(argc!=3)
     {
      printf ("Usage %s ip port\n", argv[0]);
      return 1;
     }

	/* socket create and verification */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		return 1;
	}
	bzero(&servaddr, sizeof(servaddr));

	/* assign IP, PORT */
	servaddr.sin_family = AF_INET;
	if(strcmp(localhost, argv[1]))
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);else
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(atoi(argv[2]));

	/* Binding newly created socket to given IP and verification */
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		return 1;
	}

	/* Now server is ready to listen and verification */
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		return 1;
	}

	len = sizeof(cli);

	/* Accept the data packet from client and verification */
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server acccept failed...\n");
		return 1;
	}

    bzero(buff, MAX);
    idx = -1;
    match = false;
    /* read the message from client and copy it in buffer */
    read(connfd, buff, sizeof(buff));
    printf(buff, sizeof(buff));
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
       if(buff[j]!=buff[i+j]){
        match = false;
        break;
       }
      if(match==true){
       idx = i-idx;
       break;
      }
     }
    /*clear the buffer */
    bzero(buff, MAX);

    if(match && idx!=-1)
    sprintf(buff, "%d\n", idx-1);else
    sprintf(buff, "%d\n", -1);

    printf(buff, sizeof(buff));
    /* and send that buffer to client */
    write(connfd, buff, sizeof(buff));

	/* After chatting close the socket */
	close(sockfd);

	return 0;
}

