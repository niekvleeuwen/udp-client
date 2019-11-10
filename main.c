/* 
    Author: Niek van Leeuwen
    Date:   07-11-2019
*/
#include "client.h"
#include <sensordata.h>

int send_packet(FILE *, int, struct sockaddr * ,socklen_t, int interval);

int main(int argc, char *argv[])
{
	int	sockfd, ret, portno, interval;
	char* server_ip;
	struct sockaddr_in serveraddress;

	//default values defined in client.h
	server_ip = DEFAULT_SERVER_IP;
    interval = DEFAULT_INTERVAL;
    portno = DEFAULT_PORT;

    // handle the arguments
    for (int i = 0; i < argc; i++) {
        // the user wants to use another port than the default one
        if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--port")){
			// check if the argument is a digit
			if(isNumber(argv[i+1])){
            	portno = atoi(argv[i+1]);
			}else{
				printf("The Port number provided is not correct!\n");
				exit(1);
			}
		}

        // the user wants another server ip adress
        if (!strcmp(argv[i], "-si") || !strcmp(argv[i], "--serverip")){
			// check if the argument is a valid IP Adress
			if(isValidIpAddress(argv[i+1])){
           		server_ip = argv[i+1];
			}else{
				printf("The IP Address provided is not correct!\n");
				exit(1);
			}
		}

		// the user wants another buffersize
        if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--interval")){
			// check if the argument is a digit
            if (isNumber(argv[i+1])){
                interval = atoi(argv[i+1]);
			}else{
				printf("The interval provided is not correct!\n");
				exit(1);
			}
		}

        // the user needs help
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")){
            help();
		}
    }

    // print the startup message
    startup_message(server_ip, portno, interval);

	//convert from milliseconds to seconds
	interval = interval / 1000; 

	// create a socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(0 > sockfd) 
	{
		perror("socket");
		exit(1);
	}
	
	memset(&serveraddress, 0, sizeof(serveraddress));

	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons(portno);
	serveraddress.sin_addr.s_addr = inet_addr(server_ip);
	
	printf("Client Starting service...\n");

	ret = send_packet(stdin,sockfd ,(struct sockaddr *)&serveraddress, sizeof(serveraddress), interval);
	if (ret < 0)
	{
		printf("Client Exiting - Some error occured\n");
		close(sockfd);
		exit(1);
	}
	close(sockfd);
	exit(0);
}

int send_packet(FILE *fp, int sockfd , struct sockaddr *to ,socklen_t servaddrlength, int interval)
{
	int ret;
	SENSOR_DATA sensordata;

	int i = 0;
	while(true)
	{
		//updating the struct
		sensordata.potix_return = i;
		sensordata.potiy_return = i * -1;		
		sensordata.packets_sent = i;
		// sending the read data over socket
		ret = sendto(sockfd, (void *)&sensordata, sizeof(sensordata), 0, to, servaddrlength);
		if (0 > ret)
		{
			perror("Error in sending data:\n");
			return -1;
		}
		printf("Data Sent To Server, packet number: %i\n\n", i);
		sleep(interval);
		i++;
	}
	return 0;
}

