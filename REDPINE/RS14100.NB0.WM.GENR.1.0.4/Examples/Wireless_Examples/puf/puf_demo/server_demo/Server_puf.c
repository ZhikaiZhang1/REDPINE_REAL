/*HEADER************************************************************
 Copyright (c)
 All rights reserved
 This software embodies materials and concepts that are confidential
 to Redpine Signals and is made available solely pursuant to the
 terms of a written license agreement with Redpine Signals


 Module name  : PEER APPLICATION
  
 File Name    : Server_puf.c
  
 File Description: This is a PUF demo Application at the user side for authorization.
  
 List of functions: 
    1.  main
    
/********START HEADER INCLUDE*************************************/
#include "stdio.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
/*********END HEADER FILE INCLUDE*********************************/
#define MAX_DATA            500
#define KEY_CODE_SIZE_BYTES 44
#define MAX_TCP_CONN        1
#define SIGN_LENGTH         32
//! AES Key to be used to encrypt and decrypt
#define RSI_AES_KEY1 "123456abcdefghij"

typedef struct tcp_seq
{
	unsigned char buffer[200];
}TCP_SEQ;

int processRequest(int);
signed int sock_id,fd;
void handler(int signo)
{
	printf("\nForcible Shutdown of Server\n");
	close(fd);
	close(sock_id);
	exit(1);
}

int main(int argc, char **argv)
{
	signed int dst_len = 0, ret_len = 0, tx_len = 0;
	pid_t childpid;
	struct sockaddr_in sock;
	struct sockaddr_in dst_sock;
	unsigned char data[ MAX_DATA + 1];
	char sendip[50];

	// Checking all proper Command line Arguements
	if(argc < 2)
	{
		printf("Usage ./Server_puf <lport>\n");
		exit (0);
	}

	// Creating of the TCP Socket
	if((sock_id = socket(AF_INET, SOCK_STREAM, 0)) == -1)  
	{
		while(1);
	}

	//filling the structure to connect with the client 
	sock.sin_family = AF_INET;
	sock.sin_port = htons(atoi(argv[1]));
	sock.sin_addr.s_addr = htonl(INADDR_ANY);
	//sock.sin_addr.s_addr = inet_addr("192.168.2.252");
	// try to bind to the specified socket 
	if( bind( sock_id, (struct sockaddr *)&sock, sizeof(sock)) == -1)
	{
		while(1);
	}
	signal(SIGINT,handler);

	if( listen( sock_id, MAX_TCP_CONN ) != 0)
		while(1);

	printf("Listen passed\n");	

	while(1)
	{
		//Accept client request
		fd = accept( sock_id,  (struct sockaddr *)&dst_sock, (socklen_t *)&dst_len);


		strcpy(sendip,(const char *)inet_ntop(AF_INET,(void *)&dst_sock.sin_addr,sendip,sizeof(sendip)));
		printf("Connect Req from %s accepted\n",sendip);

		if((childpid=fork())==0)
		{
			close(sock_id);
			processRequest(fd);
			exit(0);
		}
		close(fd);
	}
}

int processRequest(int fd)
{ 
	signed int dst_len = 0, ret_len = 0, tx_len = 0;
	volatile unsigned int choice;
	unsigned char data[ MAX_DATA ];
	unsigned char data1[ MAX_DATA ];
	unsigned char aes_encry_data[ MAX_DATA ];
	unsigned char kci[44];
	unsigned char kck[44];
	unsigned char sign[32];
	unsigned int act_choice, i=0,j = 0, k=0;
	uint8_t option = 0, lock_state=0,  sign_len = 0;
	uint8_t data_uart[30]= {0};
	uint8_t len_uart = 0;
	TCP_SEQ tcp_snd_pkt;

	//! Receive the data
	ret_len = recv( fd, data , MAX_DATA, 0);

	//! Parse the received data as intrinsic keycode, keycode of key and signature
	memcpy(kci,data,KEY_CODE_SIZE_BYTES);
	i = i+KEY_CODE_SIZE_BYTES;
	memcpy(kck,&data[i],KEY_CODE_SIZE_BYTES);
	i = i+KEY_CODE_SIZE_BYTES;
	memcpy(sign,&data[i],32);

option_sel:
	printf("\nEnter the option:- 1)Lock 2)Unlock:");
	scanf("%d",&option);
	memset(data_uart,0,30);
	if(option == 1 && lock_state == 0)
	{
		memcpy(data_uart,"Door Locked",sizeof("Door Locked")-1);
		printf("\nDoor locked");
		lock_state = 1;
	}
	else if(option == 1 && lock_state == 1)
	{
		printf("\nDoor already locked please enter correct option!");
		goto option_sel;
	}
	else if(option == 2 && lock_state == 1)
	{
		memcpy(data_uart,"Door Unlocked",sizeof("Door Unlocked")-1);
		printf("\nDoor unlocked");
		lock_state = 0;
	}
	else if(option == 2 && lock_state == 0)
	{
		printf("\nDoor already unlocked please enter correct option!");
		goto option_sel;
	}
	else
	{
		printf("\nInvalid option please enter the correct option!");
		goto option_sel;
	}
	len_uart = strlen(data_uart);

	j=0;
	//! Form the intermediate packet (Intrinsic keycode, signature, Message)
	memcpy(&data1[j],kci,KEY_CODE_SIZE_BYTES);
	j=j+KEY_CODE_SIZE_BYTES;
	memcpy(&data1[j],sign,SIGN_LENGTH);
	j=j+SIGN_LENGTH;
	memcpy(&data1[j],data_uart,len_uart);
	j=j+len_uart;

	//! Check whether the formed packet is multiple of keysize if not make it multiple of keysize
	while((j % 16) != 0)
	{
		j++;
		data1[j-1] = ' ';
	}

	//! Encrypt the intermediate packet in AES ECB mode 
	encrypt_data_with_key_16(data1,j,aes_encry_data,RSI_AES_KEY1);
	k = 0;

	//! Form the final packet with the keycode of the key and encrypted data
	memcpy(&tcp_snd_pkt.buffer[k],kck,KEY_CODE_SIZE_BYTES);
	k=k+KEY_CODE_SIZE_BYTES;
	memcpy(&tcp_snd_pkt.buffer[k],aes_encry_data,j);
	k=k+j;

	//! Send the packet
	tx_len = send(fd, &tcp_snd_pkt, k, 0);

	if(!tx_len)
	{
		printf("error while sending\n");
		return(0);
	}
	goto option_sel;
}



