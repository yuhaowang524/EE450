#include "network.hpp"
#include "scheduling.hpp"

using namespace std;


int main_server_boot_up(int &sockfd_udp, int &sockfd_tcp) {
    bool udp_flag = true;

    // Event: Booting Up (only while starting)
    if (initiate_server_socket(PORT_SERVER_M_TCP, !udp_flag, sockfd_tcp)) {
        return -1;
    }
    if (initiate_server_socket(PORT_SERVER_M_UDP, udp_flag, sockfd_udp)) {
        return -1;
    }
    printf("Main Server is up and running.\n");

    return 0;
}


int receive_server_name(int sockfd_udp, map<char, vector<string>> &server_data_map) {
    int count = 0;
    int num_backend_server = 2;

    while (count < num_backend_server) {
        string msg;
        int port;
        char server_code = 'A' + count;
        if (server_udp_recvfrom(sockfd_udp, msg)) {
            continue;
        }

        // Event: After receiving the username list from server A or B
        get_dynamic_port(sockfd_udp, port);

        printf("Main Server received the username list from server %c using UDP over port %d.\n",
               server_code, port);
        vector<string> temp;
        msg_to_vector(msg, temp, ' ');
        server_data_map[server_code] = temp;

        count++;
    }

    return 0;
}


int main(int argc, const char *argv[]) {
    int sockfd_udp, sockfd_tcp, child_sockfd;
    map<char, vector<string>> data_map;

    main_server_boot_up(sockfd_udp, sockfd_tcp);
    if (receive_server_name(sockfd_udp, data_map)) {
        return -2;
    }

    while (initiate_server_tcp_child_sockfd(sockfd_tcp, child_sockfd) >= 0) {
        main_server_command(sockfd_udp, sockfd_tcp, child_sockfd, data_map);
    }

    close(sockfd_udp);
    close(sockfd_tcp);
    return 0;
}