#ifndef SCHEDULING_H
#define SCHEDULING_H

#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <fstream>
#include <iostream>
#include <algorithm>


class Scheduling {
public:
    Scheduling(std::string filename);

    std::string get_time(std::string name);

    std::string get_name();

private:
    std::map<std::string, std::string> schedule_map;

    std::map<std::string, std::string> build_map(std::string);
};


// following functions are implemented in server and client
void backend_server_command(int sockfd, std::string server_name, Scheduling &schedule);

void main_server_command(int sockfd_udp, int sockfd_tcp, int child_sockfd, std::map<char, std::vector<std::string>> map_data);

void client_command(int sockfd);



// helper functions
std::string remove_spaces(std::string str);

std::string parse_input(std::string input);

int find_name(std::string name, std::map<char, std::vector<std::string>> map_data, char &server_code);

int msg_to_vector(std::string msg, std::vector<std::string> &msg_vector, char delim);

int vector_to_str(std::vector<std::string> name_vector, std::string &ret, std::string delim);

int common_time_to_str(std::vector<int> common_time, std::string &ret);

int combine_common_time(std::vector<std::string> common_time_str, std::string &ret);

int parse_time_slot(std::string time_slot_str, std::vector<std::string> &ret);


// debug printer
int print_vector(std::vector<int> ret);

int print_matrix(std::vector<std::vector<std::string>> test);

int print_map(std::map<char, std::vector<std::string>> ret);

#endif