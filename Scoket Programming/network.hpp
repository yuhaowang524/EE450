#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#define BACKLOG 10 // queue limits for usernames;
#define MAXBUFFLEN 4000 // 200 usernames each with 20 characters max

#define PORT_SERVER_M_UDP "23695"
#define PORT_SERVER_M_TCP "24695"
#define PORT_SERVER_A_UDP "21695"
#define PORT_SERVER_B_UDP "22695"


int initiate_server_socket(std::string, bool is_udp, int &sockfd);
int initiate_client_tcp_socket(std::string host, std::string port, int &sockfd);
int initiate_server_tcp_child_sockfd(int sockfd, int &re_sockfd);

int main_server_tcp_send(int child_sockfd, std::string msg);
int main_server_tcp_recv(int child_sockfd, std::string &msg);

int client_udp_sendto(int sockdfd, std::string host, std::string port, std::string msg);
int server_udp_recvfrom(int sockfd, std::string &msg);

int get_dynamic_port(int sockfd, int &port);

#endif