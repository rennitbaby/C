/*
 * INCLUDING THE PROPER LIBRARIES 
 * DEPENDING ON OPERATING SYSTEM
 *
 * */
/*
#if defined(unix) || defined(__unix__) || defined(__unix)
	#define PREDEF_PLATFORM_UNIX 1				// okay we are in a unix machine
	#include <sys/socket.h>					// including unix socket headers
	#include <netdb.h>					// needed for unix socket
	#include <netinet/in.h>					// for internet protocol family
	#include <unistd.h>					// needed for unix read/write
#elif defined(__WIN32)
	#define PREDEF_PLATFORM_MS 1				// now we are in a windows machine
	#include <winsock2.h>					// including ms socket headers
	#include <io.h>						// needed for ms read/write
#endif
*/
#include <stdio.h>						// crossed platform headers
#include <string.h>						// string header, for strcpy
#include <stdlib.h>						// stardard lib, for exit()
#include <sys/socket.h>					// including unix socket headers
#include <netdb.h>					// needed for unix socket
#include <netinet/in.h>					// for internet protocol family
#include <unistd.h>					// needed for unix read/write

/*
#if PREDEF_PLATFORM_UNIX == 1	
	printf("\n[!] We are in an unix host\n");	
#elif PREDEF_PLATFORM_MS == 1
	printf("\n[!] We are in a ms host\n");
#else
	printf("\n[!] Unable to identify the host machine");
	printf("[!] Exiting...\n");
	exit(EXIT_FAILURE);
	return 1;
#endif
*/
int main(int argc, char** argv)
{
	// operating system platform check
	printf("above init var HELLO WROLD");
	// initalizing variables
	int MAX_CONN = 5;					// max connection allowed for this tcp socket server
	int yes = 1;						// used to reuse address in socket option
	char send_str[100];					// string to send 
	char recv_str[100];					// string recieved
	int listen_fd, communicate_fd;				// file descriptors used for sockets
	struct sockaddr_in serv_addr;				// struct to store socket server address info
	
	printf("after init var HELLO WROLD");
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);		// socket method from socket.h returns a file descriptor	
								// takes params, af_inet = ip fam, sock_stream = tcp type socket
								// 0 for deafult protocol for the requets socket type
	if (listen_fd == -1){
		printf("\n[!] We couldn't establish a socket descriptor for listening");
		printf("\n[!] Exiting...");
		exit(EXIT_FAILURE);
		return 1;
	}								//
	
	printf("after we got listen fd HELLO WROLD");
	// setting the socket options at socket level, not protocol lvl, with resuseable address
	// with also the address of the interger 1 for yes, lets re use addressees, and the size of that 1
	// which is different across different platforms
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (const void*) &yes, sizeof(int)) == -1){
		printf("\n[!] We couldn't set the socket option for reusable address");
		printf("\n[!] Exiting...");
		exit(EXIT_FAILURE);
		return 1;
	}
	printf("after we set sock options HELLO WROLD");
	bzero(&serv_addr, sizeof(serv_addr));			// clearing memory for declared server addy info structure
								// takes in the address of declared struct, and the size to clear
								// it will place zero value bytes in the area pointed by the address 
	serv_addr.sin_family = AF_INET;				// setting/populating up all the server address info
	serv_addr.sin_addr.s_addr = htons(INADDR_ANY);		// this script will allow any ip to connect
								// htons used to make sure right formatting before going into the struct
	serv_addr.sin_port = htons(22000);			// using port 22000, htons to ensure formatting
	
	printf("after address info setHELLO WROLD");
	// binding using the bind method provided by socket.h: binding the socket returned file descriptor 
	// with a struct pointer pointing to the address of server address struct declared earlier
	// and also need to feed the function the size of the server address struct
	bind(listen_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));		//binding address to socket
	
	listen(listen_fd, MAX_CONN);								// after binding we listen for 5 connections

	// accept is a blocking call, blocking execution until a connection is made, 
	// once made the accept method will return a new file descriptor for communication 
	// purpose with the client socket, and waits for another connection, until the max no. of connection
	// takes in the socket file descriptor we are currently listening to, 
	// a pointer to a socketaddr struct of the particular client socket, which is NULL for any, 
	// third parameter NULL for size of struct
	communicate_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);

	// infinite loop for echo operation
	while(1){
		bzero(recv_str, 100);				// clearing out echo_str that was declared earlier, notice passing in
								// its name is = to its address, because its a char array, and all arrays
								// are pointer themselves
		bzero(send_str, 100);
		read(communicate_fd, recv_str, 100);		// reading from a file descriptor, storing it in a string(char array)
								// reading 100 bytes
		printf("\n[!] Echoing back: %s\n", recv_str);	// printing info on our screen
		
		// this is where we have a BoF problem
		strcpy(send_str, recv_str);			// using strcpy to copy the recieved string to the send string var
								// w/o limit test, memory buffer could over flow
		
		// writing to the communicate file descriptor, writing it the send string
		// also inputting the size fo the send string's length plus 1, for the null terminator?
		write(communicate_fd, send_str, strlen(send_str)+1);

	}

	return 0;



	


}

