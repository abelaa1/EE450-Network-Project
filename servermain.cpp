#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define localhost "127.0.0.1"
#define UDP_PORT_A     30412
#define UDP_PORT_B     31412
#define UDP_PORT_MAIN  32412
#define TCP_PORT_MAIN  33412
#define MAX            1000000

using namespace std;

int main()
{
    //code was copied and ajusted from geeks for geeks https://www.geeksforgeeks.org/udp-server-client-implementation-c/?ref=rp
    int udp_socket;
    struct sockaddr_in udp_server_B_addr, udp_server_main_addr, udp_server_A_addr;
    char recv[MAX];

    int udp_socket_main = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket_main < 0 ) 
    { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    }

    memset(&udp_server_main_addr, 0, sizeof(udp_server_main_addr));    // make sure the struct is empty
    udp_server_main_addr.sin_family = AF_INET;
    udp_server_main_addr.sin_port   = htons(UDP_PORT_MAIN);    //port
    udp_server_main_addr.sin_addr.s_addr = inet_addr(localhost); //set IP

    int udp_socket_B = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket_B < 0 )
    { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 

    memset(&udp_server_B_addr, 0, sizeof(udp_server_B_addr));    // make sure the struct is empty
    udp_server_B_addr.sin_family = AF_INET;
    udp_server_B_addr.sin_port   = htons(UDP_PORT_B);    //port
    udp_server_B_addr.sin_addr.s_addr = inet_addr(localhost); //set IP

    int udp_socket_A = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket_A < 0 )
    { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 

    memset(&udp_server_A_addr, 0, sizeof(udp_server_A_addr));    // make sure the struct is empty
    udp_server_A_addr.sin_family = AF_INET;
    udp_server_A_addr.sin_port   = htons(UDP_PORT_A);    //port
    udp_server_A_addr.sin_addr.s_addr = inet_addr(localhost); //set IP

    if ( bind(udp_socket_main, (const struct sockaddr *)&udp_server_main_addr,  
            sizeof(udp_server_main_addr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }

    printf("The Main server is up and running using UDP on port %d.\n",UDP_PORT_MAIN);
    //code was copied and ajusted from geeks for geeks https://www.geeksforgeeks.org/udp-server-client-implementation-c/?ref=rp

    socklen_t len_server_A;
    const char *message_A = "ready";//send ready message when the server is up to both server A and server B
    sendto(udp_socket_A, (const char *)message_A, strlen(message_A), 0, (const struct sockaddr *) &udp_server_A_addr,sizeof(udp_server_A_addr));

    string country_list_A[10];//create country list for server A
    for (int i = 0; i < 10; i++)
    {
        int recv_m;
        recv_m = read(udp_socket_main,recv,1024);
        recv[recv_m] = '\0';
        string con = recv;
        country_list_A[i] = con;
    }
    
    printf("The Main server has received the country list from server A using UDP over port %d\n", UDP_PORT_A);

    socklen_t len_server_B;
    const char *message_B = "ready";//send ready message when the server is up to both server A and server B
    sendto(udp_socket_B, (const char *)message_B, strlen(message_B), 0, (const struct sockaddr *) &udp_server_B_addr,sizeof(udp_server_B_addr));

    string country_list_B[10];//create country list for server B
    for (int i = 0; i < 10; i++)
    {
        int recv_m;
        recv_m = read(udp_socket_main,recv,1024);
        recv[recv_m] = '\0';
        string con = recv;
        country_list_B[i] = con;
    }
    
    printf("The Main server has received the country list from server B using UDP over port %d\n", UDP_PORT_B);

    printf("Server A       |Server B\n");
    for (int i = 0; i < 10; i++)
    {
        if (country_list_A[i] != "" || country_list_B[i] != "")
        {
            printf("%s            | %s\n", country_list_A[i].c_str(),country_list_B[i].c_str());
        }
    }

    //got code form geeks for geeks copied it and modified it https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/?ref=lbp
    int tcp_socket, tcp_conn; 
    struct sockaddr_in tcp_server_main_addr, tcp_client_main_addr;
    socklen_t addr_len;

    // socket create and verification 
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0); 
    if (tcp_socket == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 

    memset(&tcp_server_main_addr, 0, sizeof(tcp_server_main_addr));
    tcp_server_main_addr.sin_family = AF_INET; 
    tcp_server_main_addr.sin_addr.s_addr = inet_addr(localhost); 
    tcp_server_main_addr.sin_port = htons(TCP_PORT_MAIN); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(tcp_socket, (const struct sockaddr*)&tcp_server_main_addr, sizeof(tcp_server_main_addr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
  
    // Now server is ready to listen and verification 
    if ((listen(tcp_socket, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    addr_len = sizeof(tcp_client_main_addr); 
  
    // Accept the data packet from client and verification 

    //got code form geeks for geeks copied it and modified it https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/?ref=lbp
    while((tcp_conn = accept(tcp_socket, (struct sockaddr*)&tcp_client_main_addr, &addr_len)) > 0)
    { 
        pid_t fork_con;
        fork_con = fork();//fork function to allow muiltuple connections
        if(fork_con == 0)
        { 
            while(1)
            {
                string Coming_in_Country,Coming_in_ID;
                char buff[MAX]; 
                bzero(buff, sizeof(buff)); 
                read(tcp_conn, buff, sizeof(buff));

                Coming_in_ID = buff;//get incoming ID from client file

                bzero(buff, sizeof(buff)); 
                read(tcp_conn, buff, sizeof(buff));

                Coming_in_Country = buff;//get incoming Country from client file

                printf("The Main server has received the request on User %s in %s from the client using TCP over port %d\n",Coming_in_ID.c_str(),Coming_in_Country.c_str(),TCP_PORT_MAIN );

                int in_country_A = 0;
                int in_country_B = 0;
                //check which server the country is in
                for (int i = 0; i < 10; i++)
                {
                    if (country_list_A[i] == Coming_in_Country && country_list_A[i] != "")
                    {
                        in_country_A = 1;
                    }
                }
                for (int i = 0; i < 10; i++)
                {
                    if (country_list_B[i] == Coming_in_Country && country_list_B[i] != "")
                    {
                        in_country_B = 1;
                    }
                }

                if (in_country_A == 1)
                {
                    //if in A send data to server A
                    printf("%s shows up in server A\n", Coming_in_Country.c_str());
                    printf("The Main Server has sent request from User %s to server A using UDP over port %d\n",Coming_in_ID.c_str(), UDP_PORT_A);
                    const char *message = Coming_in_Country.c_str();
                    sendto(udp_socket_A, (const char *)message, strlen(message), 0, (const struct sockaddr *) &udp_server_A_addr,sizeof(udp_server_A_addr));

                    const char *message2 = Coming_in_ID.c_str();
                    sendto(udp_socket_A, (const char *)message2, strlen(message2), 0, (const struct sockaddr *) &udp_server_A_addr,sizeof(udp_server_A_addr));

                    socklen_t len_server_A;
                    int recv_m;
                    recv_m = read(udp_socket_main,recv,1024);
                    recv[recv_m] = '\0';
                    string check = recv;
                    if (check == "USER_ID not Found")
                    {
                        //if user is not found
                        printf("The Main server has received “User ID: Not found” from server A\n");
                        printf("The Main Server has sent error to client using TCP over %d", UDP_PORT_MAIN);
                        const char *message_TCP = "UNF";
                        write(tcp_conn, message_TCP, sizeof(message_TCP));
                    }
                    else
                    {
                        //if recommendation is found
                        write(tcp_conn, recv, sizeof(recv));
                        printf("The Main server has received searching result(s) of User %s from server A\n", check.c_str());
                        printf("The Main Server has sent searching result(s) to client using TCP over port %d\n", TCP_PORT_MAIN);
                    }
                    in_country_A = 0;
                }
                else if (in_country_B == 1)
                {
                    //if in B send data to server B
                    printf("%s shows up in server B\n", Coming_in_Country.c_str());
                    printf("The Main Server has sent request from User %s to server B using UDP over port %d\n",Coming_in_ID.c_str(), UDP_PORT_B);
                    const char *message = Coming_in_Country.c_str();
                    sendto(udp_socket_B, (const char *)message, strlen(message), 0, (const struct sockaddr *) &udp_server_B_addr,sizeof(udp_server_B_addr));

                    const char *message2 = Coming_in_ID.c_str();
                    sendto(udp_socket_B, (const char *)message2, strlen(message2), 0, (const struct sockaddr *) &udp_server_B_addr,sizeof(udp_server_B_addr));

                    socklen_t len_server_B;
                    int recv_m;
                    recv_m = read(udp_socket_main,recv,1024);
                    recv[recv_m] = '\0';
                    string check = recv;
                    if (check == "USER_ID not Found")
                    {
                        //if user is not found
                        printf("The Main server has received “User ID: Not found” from server B\n");
                        printf("The Main Server has sent error to client using TCP over %d", TCP_PORT_MAIN);
                        const char *message_TCP = "UNF";
                        write(tcp_conn, message_TCP, sizeof(message_TCP));
                    }
                    else
                    {
                        //if recommendation is found
                        write(tcp_conn, recv, sizeof(recv));
                        printf("The Main server has received searching result(s) of User %s from server B\n", check.c_str());
                        printf("The Main Server has sent searching result(s) to client using TCP over port %d\n", TCP_PORT_MAIN);
                    }
                    in_country_B = 0;
                }
                else
                {
                    //if country doesnt show up
                    printf("%s does not show up in server A&B\n",Coming_in_Country.c_str());
                    printf("The Main Server has sent “%s: Not found” to the clinet using TCP over port %d\n",Coming_in_Country.c_str(), TCP_PORT_MAIN);
                    const char *message_TCP = "NF";
                    write(tcp_conn, message_TCP, sizeof(message_TCP)); 
                }
            }
        }
        else if(fork_con > 0)
        { 
            continue; 
        }  
        else if(fork_con < 0)
        { 
            printf("fork failed to create\n"); 
            exit(0);
        } 
    } 

    close(tcp_socket); //never reached 

    close(udp_socket); //never reached

    return 0;
}