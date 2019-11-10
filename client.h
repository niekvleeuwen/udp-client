/* 
    Author: Niek van Leeuwen
    Date:   07-11-2019
*/

#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <unistd.h>

#define DEFAULT_SERVER_IP "127.0.0.1"
#define DEFAULT_PORT 7755
#define DEFAULT_INTERVAL 1000

//this function prints the help message
void help() {
    printf("\n"
    "Example usage: ./upd-client --serverip 164.93.146.29 --port 7755 --interval 5 \n"
    "\n"
    "   This program allows you to send data via UDP to a server at the specified IP address and port.\n"
    "\n"
    "Options:\n"
    "\n"
    "   -si / --serverip        IP address of the server to which the packets are sent \n"
    "                           (default: 127.0.0.1)\n"
    "\n"
    "   -p / --port             Port of the server to which the packets are sent (0-65536) \n"
    "                           (default: 7755)\n"
    "\n"
    "   -i / --interval         The interval at which the packets are sent (in milliseconds)\n"
    "                           (default: 1000)\n"
    "\n"
    "   -h / --help             Show this information\n"
    "\n"
    );

    // exit after the help message
    exit(0);
}

// this function prints a nice welcome message
void startup_message(char *serverip, int portno, int interval) {
    printf("\n"
    "           _                  _ _            _     \n"
    " _   _  __| |_ __         ___| (_) ___ _ __ | |_   \n"
    "| | | |/ _` | '_ \\ _____ / __| | |/ _ \\ '_ \\| __|  \n"
    "| |_| | (_| | |_) |_____| (__| | |  __/ | | | |_   \n"
    " \\__,_|\\__,_| .__/       \\___|_|_|\\___|_| |_|\\__| \n"
    "            |_|                                    \n");
    printf("\nStarting the server\n");
    printf("\n");
    printf("Server IP addres:   %s\n", serverip);
    printf("Server Port:        %i\n", portno);
    printf("Interval:           %i\n", interval);
    printf("\n");
}

// check if a given string is a valid IP adress
bool isValidIpAddress(char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result != 0;
}

//check if a given string is a digit
bool isNumber(char *s) 
{ 
    // loop through every character until we encounter a null byte
    for (int i = 0; s[i] != '\0'; i++) 
        if (!isdigit(s[i])) 
            return false; 
  
    return true; 
} 

