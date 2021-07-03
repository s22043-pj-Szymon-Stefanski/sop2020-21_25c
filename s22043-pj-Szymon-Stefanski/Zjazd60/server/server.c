#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define IP_PROTOCOL 0
#define PORT_NO 15050
#define NET_BUF_SIZE 32
#define cipherKey 'S'
#define sendrecvflag 0
#define nofile "NIE MA TAKIEGO PLIKU"

int sockfd;

void signalHandler(int s);

void clearing(char* cl) {
	int i;
	for (i = 0; i < NET_BUF_SIZE; i++) {
		cl[i] = '\0';
	}
}


char Cipher(char ch) {
    	return ch ^ cipherKey;
}


int sendFile(FILE* fp, char* buf, int s) {
    	int i, len;
    	if (fp == NULL) {
        	strcpy(buf, nofile);
        	len = strlen(nofile);
        	buf[len] = EOF;
        	for (i = 0; i <= len; i++)
            		buf[i] = Cipher(buf[i]);
        	return 1;
    	}

    	char ch, ch2;
    	for (i = 0; i < s; i++) {
        	ch = fgetc(fp);
        	ch2 = Cipher(ch);
        	buf[i] = ch2;
        	if (ch == EOF)
            		return 1;
    		}
    	return 0;
}


int main() {

    	int sockfd, nBytes;
    	struct sockaddr_in addr_con;

    	int addrlen = sizeof(addr_con);
    	addr_con.sin_family = AF_INET;
    	addr_con.sin_port = htons(PORT_NO);
	addr_con.sin_addr.s_addr = INADDR_ANY;

    	char net_buf[NET_BUF_SIZE];
    	FILE* fp;

    	sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL);

    	if (bind(sockfd, (struct sockaddr*)&addr_con, sizeof(addr_con)) == 0) {
        	printf("\nRozpoczynam nasluchiwanie sygnalow\n");
	}

    	while (1) {
        	printf("\nCzekam na nazwe pliku...\n");

		signal(SIGINT,&signalHandler);

        	clearing(net_buf);

        	nBytes = recvfrom(sockfd, net_buf,
			NET_BUF_SIZE, sendrecvflag,
			(struct sockaddr*)&addr_con, &addrlen);

        	fp = fopen(net_buf, "r");
        	printf("\nOdebrano nazwe pliku od klienta: %s\n", net_buf);
        	if (fp == NULL) {
            		printf("\nTAKI PLIK NIE ISTNIEJE!\n");
			printf("\nSERWER KONCZY SWOJA PRACE!\n");
			return -1;
		} else {
            		printf("\nPrzesylanie pliku powiodlo sie\n");
		}

        while (1) {
            	if (sendFile(fp, net_buf, NET_BUF_SIZE)) {
                	sendto(sockfd, net_buf, NET_BUF_SIZE,
                       	sendrecvflag,
			(struct sockaddr*)&addr_con, addrlen);
                	break;
            	}

            	sendto(sockfd, net_buf, NET_BUF_SIZE,
                   	sendrecvflag,
		(struct sockaddr*)&addr_con, addrlen);
            	clearing(net_buf);
        	}
        	if (fp != NULL) {
            		fclose(fp);
		}
    	}
    	return 0;
}

void signalHandler(int s) {
    	close(sockfd);
    	printf("\nNASTAPILO ELEGANCKIE WYJSCIE Z PROCESU\n");
	exit(0);
}

