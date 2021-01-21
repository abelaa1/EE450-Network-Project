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
#define UDP_PORT_MAIN  32412
#define MAX            1000000

using namespace std;

string recon_finder(int matrix[10][100][100],int spot_country, int spot_id, int num_users, string USER_ID[100], int add)
{
    //find the recommended friend
    int not_connected[num_users];
    int if_true = 1;
    int min_user_recon = 999999999;
    string final_recon;
    //check if user is connected to all other users and return none if true
    for (int i = 0; i < num_users; i++)
    {
        not_connected[i] = matrix[spot_country][spot_id][i];
        if (matrix[spot_country][spot_id][i] == 0 && spot_id != i)
        {
            if_true = 0;
        }
    }
    if (if_true == 1)
    {
        return("None");
    }

    //find the most common neighbors and creat an array of common neighbors
    int common_n = 0;
    int num_common_n[num_users];
    int common_n_count = 0;
    int max_common_n = 0;
    for (int i = 0; i < num_users; i++)
    {
        if (not_connected[i] == 0)
        {
            for (int j = 0; j < num_users; j++)
            {
                if (matrix[spot_country][spot_id][j] == 1 && matrix[spot_country][i][j] == 1 && spot_id != i)
                {
                    common_n_count = common_n_count + 1;
                    common_n = 1;
                    if (max_common_n < common_n_count)
                    {
                        max_common_n = common_n_count;
                    }
                }
            }
            num_common_n[i] = common_n_count;
            common_n_count = 0;
        }
    }

    //check to find the which user has the most common neighbors and return that user, includes tie breaker
    int max_common_n_pos[num_users];
    if (common_n == 1)
    {
        for (int i = 0; i < num_users; i++)
        {
            if (num_common_n[i]==max_common_n)
            {
                max_common_n_pos[i] = 1;
            }
            else
            {
                max_common_n_pos[i] = 0;
            }
        }
        for (int i = 0; i < num_users; i++)
        {
            if (max_common_n_pos[i] == 1)
            {
                if (stoi(USER_ID[i + add]) < min_user_recon)
                {
                    min_user_recon = stoi(USER_ID[i + add]);
                }
            }
        }
        final_recon = to_string(min_user_recon);
        return(final_recon);
    }

    //if no common neighbors are found find the users with most connections, includes tie breaker
    int max = 0;
    int count = 0;
    int recon[num_users];
    int max_user_pos[num_users];
    if (common_n == 0)
    {
        for (int i = 0; i < num_users; i++)
        {
            for (int j = 0; j < num_users; j++)
            {
                if (spot_id != i)
                {
                    count = count + matrix[spot_country][i][j];
                    if (count > max)
                    {
                        max = count;
                    }
                }
            }
            recon[i] = count;
            count = 0;
        }
        for (int i = 0; i < num_users; i++)
        {
            if (recon[i]==max)
            {
                max_user_pos[i] = 1;
            }
            else
            {
                max_user_pos[i] = 0;
            }
        }
        for (int i = 0; i < num_users; i++)
        {
            if (max_user_pos[i] == 1)
            {
                if (stoi(USER_ID[i + add]) < min_user_recon)
                {
                    min_user_recon = stoi(USER_ID[i + add]);
                }
            }
        }
        final_recon = to_string(min_user_recon);
        return(final_recon);
    }
    return(0); //never reached
}

string split_front(string str)
{
    //find the first user in each line to create an array of user IDs
    string word = ""; 
    for(auto x : str) 
    { 
        if(x == ' ')
        {
            if(word != "")
            {
                //after find USER ID, code breaks and returns it
                break;
            }
        } 
        else
        {
            word = word + x;
        } 
    }  

    return(word);
}

int split_and_check(string con, string id)
{
    //check if any given users is connected to another user/ used to create connection matrix
    string word = ""; 
    for(auto x : con) 
    { 
        if(x == ' ')
        {
            if(word != "")
            {
                if ((word == id) && (split_front(con) != id))
                {   
                    return(1); //if connected
                }
                word = "";
            }
        } 
        else
        {
            word = word + x;
        } 
    }  
    if ((word == id) && (split_front(con) != id))
    {
        return(1);//if connected
    }
    
    return(0);//if not connected
}

int main()
{
    //get the number of lines in the data file
    string line;
    ifstream myfile("data1.txt");
    int data = 0;
    int num_country = 0;
    if(myfile.is_open())
    {
        while(getline(myfile, line))
        {
            if (line[0] == '1' || line[0] == '2' || line[0] == '3' || line[0] == '4' || line[0] == '5' || line[0] == '6' || line[0] == '7' || line[0] == '8' || line[0] == '9')
            {
                data = data + 1; //count data lines
            }
            else
            {
                num_country = num_country + 1; //count number of countries
            }
        }
        myfile.clear();
        myfile.close();
    }
    else 
    {
        cout << "Unable to open file" << endl;
    }
    //once we have the number of lines we can create the map by making an array of [x] by [x]
    
    //creates 2 arrays of IDs and connections that correspond to each other.
    string USER_ID[1000];
    string CONNECTIONS[data];
    string COUNTRY_N[num_country];
    unsigned int num_users[num_country];
    myfile.open("data1.txt");
    unsigned int x = 0;
    unsigned int i = 0;
    unsigned int y = 0;
    if(myfile.is_open())
    {
        while(getline(myfile, line))
        {
            if (line[0] == '1' || line[0] == '2' || line[0] == '3' || line[0] == '4' || line[0] == '5' || line[0] == '6' || line[0] == '7' || line[0] == '8' || line[0] == '9')
            {
                CONNECTIONS[i] = line; //entire line including connections
                USER_ID[i] = split_front(line); //just the user
                y = y + 1;
                i = i + 1;
            }
            else
            {
                COUNTRY_N[x] = line; //country go into country array
                if (x != 0)
                {
                    num_users[x - 1] = y;
                }
                y = 0;
                x = x + 1;
            }
        }
        num_users[x - 1] = y;
        myfile.close();
    }
    else 
    {
        cout << "Unable to open file" << endl;
    }
    //create 2D matrix to hold map information
    int social_map[10][100][100];
    int countw = 0;
    int countj = 0;
    for (int t = 0; t < num_country; t++)
    {
        for (int j = 0; j < num_users[t]; j++)
        {
            for (int w = 0; w < num_users[t]; w++)
            {
                social_map[t][w][j] = split_and_check(CONNECTIONS[w + countw],USER_ID[countj]);
            }
            countj = countj + 1;
        }
        countw = countj;
    }

    //print in order to debug
    
    /*
    for (int t = 0; t < num_country; t++)
    {
        cout << COUNTRY_N[t] <<endl;
        for (int j = 0; j < num_users[t]; j++)
        {
            for (int w = 0; w < num_users[t]; w++)
            {
                cout << social_map[t][j][w];
            }
            cout <<endl;
        }
    }
    */

    //udp connection is being establish
    //code was copied and ajusted from geeks for geeks https://www.geeksforgeeks.org/udp-server-client-implementation-c/?ref=rp
    int udp_socket;
    struct sockaddr_in udp_server_A_addr, udp_server_main_addr;
    char recv[MAX];
    string Coming_in_Country,Coming_in_ID;

    int udp_socket_main = socket(AF_INET, SOCK_DGRAM, 0);
    if(udp_socket_main < 0 )
    { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 

    memset(&udp_server_main_addr, 0, sizeof(udp_server_main_addr));    // make sure the struct is empty
    udp_server_main_addr.sin_family = AF_INET;
    udp_server_main_addr.sin_port   = htons(UDP_PORT_MAIN);    //port
    udp_server_main_addr.sin_addr.s_addr = inet_addr(localhost); //set IP

    int udp_socket_A = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket_A < 0 ) 
    { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    memset(&udp_server_A_addr, 0, sizeof(udp_server_A_addr));    // make sure the struct is empty
    udp_server_A_addr.sin_family = AF_INET;
    udp_server_A_addr.sin_port   = htons(UDP_PORT_A);    //port
    udp_server_A_addr.sin_addr.s_addr = inet_addr(localhost);

    if ( bind(udp_socket_A, (const struct sockaddr *)&udp_server_A_addr,  
            sizeof(udp_server_A_addr)) < 0 ) 
        { 
            perror("bind failed"); 
            exit(EXIT_FAILURE); 
        }

    printf("The Server A is up and running using UDP on port %d.\n",UDP_PORT_A);

    socklen_t len_server_A;
    int recv_m;
    recv_m = read(udp_socket_A,recv,1024);
    recv[recv_m] = '\0';
    //code was copied and ajusted from geeks for geeks https://www.geeksforgeeks.org/udp-server-client-implementation-c/?ref=rp

    // sending country list over to main server
    socklen_t len_server_main = sizeof(udp_server_main_addr);
    for (int i = 0; i < num_country; i++)
    {
        const char *message = COUNTRY_N[i].c_str();
        sendto(udp_socket_main, (const char *)message, strlen(message), 0, (const struct sockaddr *) &udp_server_main_addr,len_server_main);
    }
    for (int i = num_country; i < (num_country+(10 - num_country)); i++)
    {
        const char *message = "";
        sendto(udp_socket_main, (const char *)message, strlen(message), 0, (const struct sockaddr *) &udp_server_main_addr,len_server_main);
    }

    printf("The server A has sent a country list to Main Server\n");

    while(1)
    {
        socklen_t len_server_main = sizeof(udp_server_main_addr);
        string user_country, user_in_id;
        int recv_m;
        recv_m = read(udp_socket_A,recv,1024);
        recv[recv_m] = '\0';
        user_country = recv;//country being reviced by main

        recv_m = read(udp_socket_A,recv,1024);
        recv[recv_m] = '\0';
        user_in_id = recv;//user being reviced by main

        printf("The server A has received request for finding possible friends of User %s in %s\n", user_in_id.c_str(),user_country.c_str());

        //checking if country is in this server and getting country spot
        int is_country = 0;
        int spot_country = 0;
        for (int i = 0; i < num_country; i++)
        {
            if (COUNTRY_N[i] == user_country)
            {
                is_country = 1;
                spot_country = i;
            }
        }

        //checking if user is in the country
        int is_user = 0;
        int spot_id = 0;
        int spot_id_count = 0;
        int add = 0;
        if (spot_country == 0)
        {
            for (int i = 0; i < num_users[spot_country]; i++)
            {
                if (user_in_id == USER_ID[i])
                {
                    spot_id = i;
                    is_user = 1;
                    //if user found save spot and move on for first country
                }
            }
            if (is_user == 0)
            {
                //if user not found
                const char *message = "USER_ID not Found";
                sendto(udp_socket_main, (const char *)message, strlen(message), 0, (const struct sockaddr *) &udp_server_main_addr,len_server_main);
                printf("User %s does not show up in %s\n", user_in_id.c_str(),user_country.c_str());
                printf("The server A has sent “User %s not found” to Main Server\n", user_in_id.c_str());
            }
        }
        else
        {
            //if user found save spot and move on for any other country
            for (int i = 0; i <= spot_country-1; i++)
            {
                add = add + num_users[i];
            }
            for (int j = add; j < add + num_users[spot_country]; j++)
            {
                if (user_in_id == USER_ID[j])
                {
                    spot_id = spot_id_count;
                    is_user = 1;
                }
                spot_id_count = spot_id_count + 1;
            }
            if (is_user == 0)
            {
                //if user not found
                const char *message = "USER_ID not Found";
                sendto(udp_socket_main, (const char *)message, strlen(message), 0, (const struct sockaddr *) &udp_server_main_addr,len_server_main);
                printf("User %s does not show up in %s\n", user_in_id.c_str(),user_country.c_str());
                printf("The server A has sent “User %s not found” to Main Server\n", user_in_id.c_str());
            }
        }

        if (is_user == 1 && is_country == 1)
        {
            //if all checks out look for recommendation and send over to mainserver
            string all;
            all = recon_finder(social_map,spot_country,spot_id,num_users[spot_country],USER_ID, add);
            const char *message = all.c_str();
            sendto(udp_socket_main, (const char *)message, strlen(message), 0, (const struct sockaddr *) &udp_server_main_addr,len_server_main);
            printf("The server A is searching possible friends for User %s ...\n", user_in_id.c_str());
            printf("Here are the results: User %s...\n", all.c_str());
            printf("The server A has sent the result(s) to Main Server\n");
        }
    }

    return 0;
}