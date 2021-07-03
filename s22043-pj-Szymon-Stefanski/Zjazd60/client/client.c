#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define IP_PROTOCOL 0
#define IP_ADDRESS "127.0.0.1"
#define PORT_NO 15050
#define NET_BUF_SIZE 32
#define cipherKey 'S'
#define sendrecvflag 0

void clearing(char* cl) {
    	int i;
    	for (i = 0; i < NET_BUF_SIZE; i++) {
        	cl[i] = '\0';
	}
}

char Cipher(char ch) {
    	return ch ^ cipherKey;
}

int readFile(char* buf, int s)
{
    int i;
    char ch;
    for (i = 0; i < s; i++) {
        ch = buf[i];
        ch = Cipher(ch);
        if (ch == EOF)
            return 1;
        else
            printf("%c", ch);
    }
    return 0;
}

int main()
{
    	int sockfd, nBytes;
    	struct sockaddr_in addr_con;

    	int addrlen = sizeof(addr_con);
    	addr_con.sin_family = AF_INET;
    	addr_con.sin_port = htons(PORT_NO);
    	addr_con.sin_addr.s_addr = inet_addr(IP_ADDRESS);

    	char net_buf[NET_BUF_SIZE];
    	FILE* fp;

    	sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL);

    	if (sockfd < 0) {
        	printf("\nNie udalo polaczyc sie z serwerem\n");
	} else {
        	printf("\nUdalo polaczyc sie z serwerem\n");
	}

    	while (1) {
        	printf("\nWpisz jaki plik chcialbys odebrac: ");
		printf("\nWcisnij kombinacje klawiszy CTRL+C by wyjsc\n");

        	scanf("%s", net_buf);

        	sendto(sockfd, net_buf, NET_BUF_SIZE,
               		sendrecvflag, (struct sockaddr*)&addr_con,
               		addrlen);

               	printf("\nUWAGA - JESLI NIC SIE NIE WYSWIETLILO TO ZNACZY");
               	printf("\nZE TAKI PLIK NIE ISTNIEJE A SERWER SIE ROZLACZYL\n");

               	printf("\nPLIK ODEBRANY, TRESC:\n");


        while (1) {
            	// Kiedy plik zostal odebrany
            	clearing(net_buf);
            	nBytes = recvfrom(sockfd, net_buf, NET_BUF_SIZE,
                              	sendrecvflag, (struct sockaddr*)&addr_con,
                              	&addrlen);
            	// Konczenie procesu
            	if (readFile(net_buf, NET_BUF_SIZE)) {
                	break;
            	}
        }
        printf("\nZAKONCZONO OPERACJE POMYSLNIE\n");
    }
    return 0;
}

