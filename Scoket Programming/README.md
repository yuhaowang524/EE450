# EE450 Project Socket Programming, Spring 2023

## Full Name
Yuhao Wang

## Student ID
5779881695

## Introduction
This socket program is designed to fasten meeting scheduling process. The blueprint consists of one main server (server
M), 2 backend servers (server A and server B), and a client. each one of the backend servers will read a txt file and store
schedules of individuals in their end. Then backend servers will forward their name list to the main server through UDP 
connection. Both the main server and backend servers implement a scheduling algorithm to find the common time. Client can 
send/receive messages to/from the main server so that common time slots will be obtained from the main server. This project
is written in C++ and complied with -std=c++11

## Structures
### `network.hpp`, `network.cpp`
utility files that work with the TCP/UDP initiation and connections. Work cited from Beej's Guide to Network Programming

### `scheduling.hpp`, `scheduling.cpp`
* `Scheduling` class  
  Extract name and schedule from input txt file, a basic `map<string, string>` data structure is used to store those data
* Utility functions for backend servers, main server, and client in while loop
  * `backend_server_command(int sockfd, string server_name, Scheduling schedule)`
  * `main_server_command(int sockfd_udp, int sockfd_tcp, int child_sockfd, map<char, vector<string>> map_data)`
  * `void client_command(int sockfd)`
* Helper functions to return intermediate values

### `serverA.cpp`, `serverB.cpp`
Initiate backend server A and B UDP connections. The loop is illustrated below:
* boot up backend server and initiate UDP protocols
* send name list read from txt file to the main server
* accept messages from the main server
* runs over scheduling algorithm to find the common time interval
* send the result back to the main server

### `serverM.cpp`
Initiate main server TCP/UDP connections. The loop is illustrated below:
* boot up main server and initiate UDP/TCP protocols
* receive name lists from backend server A and B and store names into a `map<string, string>` data structure
* wait for incoming messages from client
  * only valid names will be forwarded to corresponding backend servers
  * invalid names will be sent back to the client 
* forward sub name list to according backend servers where their names are stored
* wait for incoming schedule messages from backend servers
* runs over scheduling algorithm again to combine the results from backend servers
* send the result back to the client

### `client.cpp`
Initiate TCP connections for client.
* send user input to the main server
* return the final result 

## Message Format Notes

### Phase 1 Boot-up
* individuals' schedule is stored inside `map<string name, string time>`, where
  * \<name> is the name of a person
  * \<time> is the schedule of a person
* name list from backend server is sent as one `string` delimited with one space

### Phase 2 Forward
* client -> main server: input `string` is delimited with one space
* main server -> backend server: name list is sent as one `string` delimited with one space

### Phase 3 Schedule
* backend server converts the received message from `string` to `vector<string>` where each element inside the vector is 
the name
* scheduling algorithm compares all candidates' schedule with a stepped time slot [\<0,1>,<1,2>,...,<99,100>] and return
common time slot in `vector<string>` if available
* backend server -> main server: result is sent as one `string` with the format of [\<start_time_0,end_time_0>,...,<start_time_i,end_time_i>]

### Phase 4 Reply
* main server -> client: different messages are sent with different headers 
  * `0:msg_body` indicates names not found
  * `1:msg_body` indicates combined schedule from main server
    * notice main server will send an empty time interval to the client even if no common time is found or no valid input is entered

## Idiosyncrasy
Make sure to enter valid name input from the client, my program does not detect the uppercase and lower case difference
for example:
* if "tom" is stored in the name list of the main server
  * entering "Tom" does not extract information about "tom" 
* make sure to enter names delimited with `one space` to avoid error
  * <pre>>>sam bob amy</pre>
* the client program will parse user input and remove all unnecessary spaces before, after and in between names 
  * <pre>>>        adam        sam      jimmy            </pre> is equivalent to
    <pre>>>adam sam jimmy</pre>
* the client program will also remove duplicate inputs, but the backend server program will not remove duplicates 
  please make sure all names in the txt file is unique  

## Reference && Citations

### Socket Programming
* Code in `network.cpp` cited from [Beej's Guide to Network Programming](https://www.beej.us/guide/bgnet/html/)
  * details are reflected at the comment section of cited functions
* [Socket Programming in C/C++ - GeeksforGeeks](https://www.geeksforgeeks.org/socket-programming-cc/?ref=leftbar-rightbar)

### Others
* [Socket in C; infinite loop](https://stackoverflow.com/questions/16005710/socket-in-c-infinite-loop)
* [Remove from the beginning of std::vector [closed]](https://stackoverflow.com/questions/40656871/remove-from-the-beginning-of-stdvector)
* [What's the most efficient way to erase duplicates and sort a vector?](https://stackoverflow.com/questions/1041620/whats-the-most-efficient-way-to-erase-duplicates-and-sort-a-vector)