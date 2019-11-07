/* 
    Author: Niek van Leeuwen
    Date:   07-11-2019
*/
#include "client.h"

#define BUFSIZE 512
#define MAXSERVERNAME 100

int send_packet(FILE *, int, struct sockaddr * ,socklen_t);

int main(int argc, char *argv[] )
{
	int	sockfd, ret, portno;
	char* server_ip = DEFAULT_SERVER_IP;
	struct	sockaddr_in serveraddress;

	// size of the buffer
    int buffersize = DEFAULT_BUFFERSIZE;

	// portnumber defined in client.h 
    portno = atoi(DEFAULT_PORT);

    // handle the arguments
    for (int i = 0; i < argc; i++) {
        // the user wants to use another port than the default one
        if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--port")){
			// check if the argument is a number
            if (isdigit(argv[i+1])){ 
                portno = atoi(argv[i+1]);
			}
		}

        // the user wants another server ip adress
        if (!strcmp(argv[i], "-si") || !strcmp(argv[i], "--serverip")){
			// check if the argument is a valid IP Adress
			if(isValidIpAddress(argv[i+1])){
           		server_ip = argv[i+1];
			}
		}

		// the user wants another buffersize
        if (!strcmp(argv[i], "-bs") || !strcmp(argv[i], "--buffersize")){
			// check if the argument is a number
            if (isdigit(argv[i+1])){
                buffersize = htons(atoi(argv[i+1]));
			}
		}

        // the user needs help
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")){
            help();
		}
    }

    // print the startup message
    startup_message(server_ip, portno, buffersize);

	sockfd = socket( AF_INET, SOCK_DGRAM, 0 );
	if(0 > sockfd ) 
	{
		perror( "socket" );
		exit (1);
	}
	
	memset( &serveraddress, 0, sizeof(serveraddress) );
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons(portno);
	serveraddress.sin_addr.s_addr = inet_addr(server_ip);
	
	printf("Client Starting service\n");

	ret = send_packet(stdin,sockfd ,(struct sockaddr *)&serveraddress,
			sizeof(serveraddress));
	if (0 > ret)
	{
		printf("Client Exiting - Some error occured\n");
		close(sockfd);
		exit(1);
	}
	close(sockfd);
	exit(0);
}

int send_packet(FILE *fp, int sockfd , struct sockaddr *to ,socklen_t length)
{
	char sendbuf[BUFSIZE], recvbuf[BUFSIZE],servername[MAXSERVERNAME];
	char *cptr;
	int ret, numbytes, slen;
	socklen_t structlen;
	struct sockaddr_in serveraddr;
	
	for(;;)
	{
		
		printf("Enter Data For the server or press CTRL-D to exit\n");
		// reading data from the keyboard
		cptr = fgets(sendbuf,BUFSIZE,fp);
		if (NULL == cptr)
		{
			printf("Possible error or end of file\n");
			return 0;
		}
		slen = strlen (sendbuf);
		// sending the read data over socket
		ret = sendto(sockfd,sendbuf,slen,0,to,length);
		if (0 > ret)
		{
			perror("Error in sending data:\n");
			return -1;
		}
		printf("Data Sent To Server\n");
		structlen = sizeof(serveraddr);
		numbytes = recvfrom(sockfd,recvbuf,BUFSIZE,0, (struct sockaddr*)&serveraddr,&structlen);
		if (0 > numbytes)
		{
			perror("Error in receiving data:\n");
			return -1;
		}
		printf("Data Received from server %s:\n", inet_ntop(AF_INET,&serveraddr.sin_addr,
				servername,sizeof(servername)));
		// writing to stdout
		write(1,recvbuf,numbytes); 
	}

}

