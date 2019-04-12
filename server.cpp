// Sockets stuff
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

// Other stuff
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <cassert>
#include "sockets.h"

struct Node {
   std::string s;
   struct Node* next;
};

// TODO:
void append(Node* head, char c);
void server_append(Node* head, int socket);
Node* receive_list(int socket);

void print_list(Node* head);
int setup_server_socket(int port);

int main(int argc, char * argv[])
{
   if(argc != 2)
   {
      std::cout << "USAGE: ./server <port>" << std::endl;
      return 1;
   }
   int port = atoi(argv[1]);
   int socket = setup_server_socket(port);

   Node * head = receive_list(socket);

   std::cout << "Received list: " << std::endl;
   print_list(head); 

   server_append(head, socket);

   std::cout << "Called server append: " << std::endl;
   print_list(head);

   close(socket);
   return 0;
}

void append(Node* head, char c) {
    // TODO
}

void server_append(Node* head, int socket) {
    // TODO
}

Node * receive_list(int socket) {
    // TODO
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

int setup_server_socket(int port)
{
   // Create socket
   int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if(sockfd == -1)
   {
      std::cout << "Error creating socket: " << strerror(errno) << std::endl;
      return -1;
   }
   
   // Set socket options (setsockopt)
   int on = 1;
   int sockopt_return_val = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
   assert(sockopt_return_val != -1);

   // Create a sockaddr_in struct for the proper port and bind() to it.
   struct sockaddr_in addr;
   socklen_t length = socklen_t(sizeof(addr));
   memset(&addr, 0, length);                   // Zero socket memory
   addr.sin_family = AF_INET;                  // Set socket family
   addr.sin_addr.s_addr = htonl(INADDR_ANY);   // Specify socket address (hostname)
   addr.sin_port = htons(port);                // Set port value

   // Bind to socket
   if(bind(sockfd, (struct sockaddr*) &addr, length) == -1)
   {
      std::cout << "Error binding to socket" << std::endl;
      return -1;
   }

   // Get the assigned port number
   if(getsockname(sockfd, (struct sockaddr*) &addr, &length) == -1)
   {
      std::cout << "Error getting port number" << std::endl;
      return -1;
   }

   // Begin listening for incoming connections.
   if(listen(sockfd, 10) == -1)
   {
      std::cout << "Error listening" << std::endl;
      return -1;
   }
      
   // Accept a connection to a client
   int newconnectionfd = accept(sockfd, (struct sockaddr *) 0, (unsigned *) 0);
   if(newconnectionfd == -1)
   {
      std::cout << "Error accepting connection: " << strerror(errno) << std::endl;
     return -1;
   }

   return newconnectionfd;
}
