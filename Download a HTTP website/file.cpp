//including all header files
#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

int main()
{
	char dom[100];
	
	//asking user for a domain name
	cout << "Enter domain name: ";
	cin >> dom;

	//getting the IP address from the domain name
	struct hostent *ghbn;
	ghbn = gethostbyname(dom);
	char* IP = inet_ntoa(*((struct in_addr *)(ghbn->h_addr)));
	cout << "IP address of " << dom << " is " << IP << endl;
	
	
	
	const char *server_ip = IP; 
	const int server_port = 80;      

	//create a socket
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		perror("Error creating socket");
		return 1;
	}

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(server_port);
	
	if (inet_pton(AF_INET, server_ip, &server_address.sin_addr) <= 0) {
		perror("Invalid address or address not supported");
		return 1;
	}

	//connecting to the socket
	if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
		perror("Error connecting to server");
		return 1;
	}


	//sending message to connected server
	char message[256000];
	sprintf(message, "GET / HTTP/1.0\r\n\r\n");

	if (send(client_socket, message, strlen(message), 0) == -1) {
		perror("Error sending data");
		return 1;
	}
	printf("Message sent to server: %s\n", message);
	
	char recieve[2560];

	//recieving the message from server
	string str;
	printf("Message recieved from server: \n");

	//since we dont know the length of the HTML file we grab the file by parts
	// and append it into the string str.
	while(recv(client_socket,recieve,sizeof(recieve),0)>0)
	{
		str = str + recieve;
		memset(recieve,0,sizeof(recieve));
	}

	//closing connection.
	close(client_socket);	
	

	//Entering name of file to be saved in the system.
	string filename;	
	cout << endl << "Enter Filename: ";
	cin >> filename;

	//saving the file as .html file
	string name = filename + ".html";

	//creating a file
	ofstream inputfile(name);
	
	//writing into the file
	inputfile << str;	
	
	return 0;
}
