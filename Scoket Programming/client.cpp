#include "network.hpp"
#include "scheduling.hpp"

using namespace std;


void boot_cout() {
    printf("Client is up and running.\n");
}


int main(int argc, char const *argv[]) {
    boot_cout();

    while (true) {
        int sockfd;

        // Event: Booting Up
        if (initiate_client_tcp_socket("localhost", PORT_SERVER_M_TCP, sockfd)) {
            return -1;
        }
        client_command(sockfd);
        close(sockfd);
    }
    return 0;
}