#include <stdio.h>
#include <string>
#include <ctype.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <iostream>

#define localhost "127.0.0.1"
#define TCP_PORT_MAIN  33412
#define MAX            1000000

using namespace std;

void func(int tcp_socket, sockaddr_in tcp_server_main_addr) 
{
    string User, Country;
    printf("Please enter the User ID: \n");
    cin >> User;
    const char *message = User.c_str();//send user to mainserver
    write(tcp_socket, message, sizeof(message)); 

    printf("Please enter Country Name: \n");
    cin >> Country;
    const char *message1 = Country.c_str();//send country to mainserver
    write(tcp_socket, message1, sizeof(message1)); 

    printf("The client has sent User %s and %s to Main Server using TCP\n",User.c_str(),Country.c_str());
    char buff[MAX]; 
    bzero(buff, sizeof(buff)); 
    read(tcp_socket, buff, sizeof(buff));//recieved result and put it into a string to check
    string check = buff;
    if (check == "NF")//if country not found
    {
        printf("%s not found\n",Country.c_str());
    }
    else if (check == "UNF")//if user not found
    {
        printf("User %s not found\n",User.c_str());
    }
    else//if recommendation is found
    {
        printf("The client has received results from Main Server: User %s is/are possible friend(s)of User %s in %s\n",check.c_str(),User.c_str(),Country.c_str());
    }
}

int main() 
{ 
    //got code form geeks for geeks copied it and modified it https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/?ref=lbp
    printf("The client is up and running\n");

    int tcp_socket, tcp_conn; 
    struct sockaddr_in tcp_server_main_addr, tcp_client_main_addr;
    socklen_t addr_len;
  
    // socket create and varification 
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0); 
    if (tcp_socket == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    memset(&tcp_server_main_addr, 0 ,sizeof(tcp_server_main_addr)); 
  
    // assign IP, PORT 
    tcp_server_main_addr.sin_family = AF_INET; 
    tcp_server_main_addr.sin_addr.s_addr = inet_addr(localhost); 
    tcp_server_main_addr.sin_port = htons(TCP_PORT_MAIN); 
  
    // connect the client socket to server socket 
    if (connect(tcp_socket, (struct sockaddr*)&tcp_server_main_addr, sizeof(tcp_server_main_addr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
    {
        //set dynamic port
        int getsock = getsockname(tcp_socket,(struct sockaddr*)&tcp_server_main_addr,&addr_len);
        if(getsock == -1)
        {
            printf("getsockname didnt work\n");
            exit(1);
        }
    } 
    //got code form geeks for geeks copied it and modified it https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/?ref=lbp
  
    // function for chat 
    while(1)
    {
        func(tcp_socket, tcp_server_main_addr); 
    }
  
    // close the socket 
    close(tcp_socket); 
} 