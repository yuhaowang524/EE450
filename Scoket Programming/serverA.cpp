#include "network.hpp"
#include "scheduling.hpp"

using namespace std;

int main(int argc, const char *argv[]) {
    bool udp_flag = true;
    string file_name = "a.txt";

    // Event: Booting Up (Only while starting)
    Scheduling server_A = Scheduling(file_name);
    string server_name_str = server_A.get_name();
    int sockfd;
    if (initiate_server_socket(PORT_SERVER_A_UDP, udp_flag, sockfd)) {
        return -1;
    }
    printf("Server A is up and running using UDP on port %s.\n", PORT_SERVER_A_UDP);

    // Event: After sending the list of usernames to the main server
    client_udp_sendto(sockfd, "localhost", PORT_SERVER_M_UDP, server_name_str);
    printf("Server A finished sending a list of usernames to Main Server.\n");

    while(true) {
        backend_server_command(sockfd, "A", server_A);
    }

    close(sockfd);
    return 0;
}