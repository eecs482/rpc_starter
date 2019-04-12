// Sockets stuff
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

// Other
#include <iostream>
#include <string.h>
#include <stdlib.h>

struct Node {
   std::string s;
    struct Node* next;
};

// TODO:
void send_list(int socket, Node* head);
void client_append(int socket, char appendChar);

int list_length(Node* head);
void print_list(Node* head);
void send_message(int sock, void * data, unsigned int bytes_to_send);
int setup_client_socket(const char * hostname, int port);

int main(int argc, char *argv[]) {
	if (argc < 3) {
      std::cout << "USAGE: ./client <server hostname> <port>" << std::endl;
      exit(1);
	}
   
   const char * hostname = argv[1];
   int port = atoi(argv[2]);

	int socket = setup_client_socket(hostname, port);
   
   Node* head = new Node();
   head->s = "cat";
   head->next = new Node();
   head->next->s = "dog";
   head->next->next = nullptr;

   std::cout << "My list is: " << std::endl;
   print_list(head);

   std::cout << "Sending list" << std::endl;
   send_list(socket, head);

   std::cout << "Appending 's' to list" << std::endl;
   client_append(socket, 's');

   close(socket);
	return 0;
}


void send_list(int socket, Node* head) {
    // TODO
}

void client_append(int socket, char appendChar) {
    // TODO
}

int setup_client_socket(const char * hostname, int port)
{
	// Create socket
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == -1) {
      std::cout << "Error getting socket: " << strerror(errno) << std::endl;
		return -1;
	}

   // gethostbyname returns a struct including the network address of
   // the specified host (does a DNS lookup)
   struct hostent * hp = gethostbyname(hostname);
   if(hp == nullptr)
   {
      std::cout << "Unknown host: " << hostname << std::endl;
      return -1; 
   }

	// Connect socket using name specified by command line. */
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	memcpy(reinterpret_cast<char *>(&server.sin_addr),
			reinterpret_cast<char *>(hp->h_addr), hp->h_length);
	server.sin_port = htons(port);

	// Connect to the server
	if (connect(sock, reinterpret_cast<struct sockaddr *>(&server),
			sizeof(server)) == -1) {
      std::cout << "Error connecting to server: " << strerror(errno) << std::endl;
		return -1;
	}

	return sock;
}

void send_message(int sock, void * data, unsigned int bytes_to_send)
{
	int total_bytes_sent = 0, bytes_sent = 0;
	
   while (bytes_to_send != 0) {
		// Attempt to send all unsent message bytes remaining
      bytes_sent = send(sock, (char*)data + total_bytes_sent, bytes_to_send, 0);
		if (bytes_sent == -1) {
         std::cout << "Error sending message: " << strerror(errno) << std::endl;
			return;
		}

		bytes_to_send -= bytes_sent;
	   total_bytes_sent += bytes_sent;
	}

}

int list_length(Node* head) {
	Node * curr = head;
	int length = 0;
	while(curr) {
		++length;
		curr = curr->next;
	}
   return length;
}

void print_list(Node * head) {
   Node * curr = head;
   int nodeNum = 0;
   while(curr)
   {
      std::cout << "list[" << nodeNum << "] : " << curr->s << std::endl;
      curr = curr->next;
      ++nodeNum;
   }
}
