#include "network.hpp"

#ifdef __DEBUG__
#include <arpa/inet.h> // this needs attention
#endif


// following codes are copied (with modification to work for the requirement
// of EE450 socket project) from Beej’s Guide to Network Programming
// CHAPTER 5. SYSTEM CALLS OR BUST, Page 19, 22, 31
int initiate_server_socket(std::string port, bool udp_flag, int &sockfd) {
    struct addrinfo hints;
    struct addrinfo *servinfo;
    struct addrinfo *p;
    int connection_status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    if (udp_flag) {
        hints.ai_socktype = SOCK_DGRAM;
    } else {
        hints.ai_socktype = SOCK_STREAM;
    }
    hints.ai_flags = AI_PASSIVE;

    //connection request
    connection_status = getaddrinfo(NULL, port.c_str(), &hints, &servinfo);
    if (connection_status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(connection_status));
        return -1;
    }

    // loop through and bind to the first address available
    int yes = 1;
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("SERVER_INFO: socket");
            continue;
        }

        // skip "Address Already in use" error message
        // todo:
        //  potential bug: replace "yes" with type int
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("SERVER_INFO: setsockopt");
            return -2;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("SERVER_INFO: bind");
            continue;
        }
        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL) {
        fprintf(stderr, "SERVER_INFO: failed to bind\n");
        return -3;
    }

    if (!udp_flag && listen(sockfd, BACKLOG) == -1) {
        fprintf(stderr, "SERVER_INFO: failed to listen\n");
        return -4;
    }

    return 0;
}


// following codes are copied (with modification to work for the requirement
// of EE450 socket project) from Beej’s Guide to Network Programming
// CHAPTER 6. CLIENT-SERVER BACKGROUND, Page 33
int initiate_client_tcp_socket(std::string host, std::string port, int &sockfd) {
    struct addrinfo hints;
    struct addrinfo *servinfo;
    struct addrinfo *p;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s/n", gai_strerror(rv));
        return -1;
    }

    // loop through and connect to the first result found
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("CLIENT_INFO: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("CLIENT_INFO: connect");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL) {
        fprintf(stderr,"CLIENT_INFO: client failed to connect\n");
        return -2;
    }

    return 0;
}


// following codes are copied (with modification to work for the requirement
// of EE450 socket project) from Beej’s Guide to Network Programming
// CHAPTER 5. SYSTEM CALLS OR BUST, Page 24
// CHAPTER 6. CLIENT-SERVER BACKGROUND, Page 31
int initiate_server_tcp_child_sockfd(int sockfd, int &sockfd_dup) {
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof their_addr;

    // return a child socket file descriptor for one single connection
    int child_sockfd = accept(sockfd, (struct sockaddr *) &their_addr, &addr_size);
    if ((child_sockfd) == -1) {
        perror("SERVER_TCP_ERROR: accept");
        return -1;
    }
    sockfd_dup = child_sockfd;
    return 0;
}


// following codes are copied (with modification to work for the requirement
// of EE450 socket project) from Beej’s Guide to Network Programming
// CHAPTER 6. CLIENT-SERVER BACKGROUND, Page 31
int main_server_tcp_send(int sockfd, std::string msg) {
    int msg_len = msg.size();
    int flags = 0;
    ssize_t bytes_sent;
    bytes_sent = send(sockfd, msg.c_str(), msg_len, flags);
    if (bytes_sent == -1) {
        perror("ERROR: send");
        return -1;
    }
    return 0;
}


// following codes are copied (with modification to work for the requirement
// of EE450 socket project) from Beej’s Guide to Network Programming
// CHAPTER 6. CLIENT-SERVER BACKGROUND, Page 33, 34
int main_server_tcp_recv(int sockfd, std::string &msg) {
    int flags = 0;
    char buf[MAXBUFFLEN];
    ssize_t num_bytes;
    num_bytes= recv(sockfd, buf, MAXBUFFLEN - 1, flags);
    if (num_bytes == -1) {
        perror("ERROR: recv");
        return -1;
    }

    buf[num_bytes] = '\0';
    msg = buf;

    return 0;
}


// following codes are copied (with modification to work for the requirement
// of EE450 socket project) from Beej’s Guide to Network Programming
// CHAPTER 9. MAN PAGES, Page 104
int server_udp_recvfrom(int sockfd, std::string &msg) {
    int num_bytes;
    int flags = 0;
    char buf[MAXBUFFLEN];
    struct sockaddr_storage their_addr;
    socklen_t addr_size;

    addr_size = sizeof their_addr;
    num_bytes = recvfrom(sockfd, buf, MAXBUFFLEN - 1, flags, (struct sockaddr *) &their_addr, &addr_size);
    if (num_bytes == -1) {
        perror("ERROR: recvfrom failed");
        return -1;
    }

    buf[num_bytes] = '\0';
    msg = buf;

    return 0;
}


// following codes are copied (with modification to work for the requirement
// of EE450 socket project) from Beej’s Guide to Network Programming
// CHAPTER 9. MAN PAGES, Page 110
int client_udp_sendto(int sockdfd, std::string host, std::string port, std::string msg) {
    struct addrinfo hints;
    struct addrinfo *servinfo;
    struct addrinfo *p;
    int rv;
    int num_bytes;
    int flags = 0;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    rv = getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo);
    if (rv != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    // loop through all the results and make a socket
    for (p = servinfo; p != NULL; p = p->ai_next) {
        num_bytes = sendto(sockdfd, msg.c_str(), msg.size(), flags, p->ai_addr, p->ai_addrlen);
        if (num_bytes == -1) {
            perror("CLIENT_INFO: sendto");
            continue;
        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "ERROR: client udp failed to send\n");
        return -2;
    }

    if (num_bytes < (int) msg.size()) {
        fprintf(stderr, "ERROR: client udp sends %d bytes, %d bytes expected\n", num_bytes, (int) msg.size());
        return -3;
    }

    return 0;
}


int get_dynamic_port(int sockfd, int &port) {
    struct sockaddr_in client;
    socklen_t client_size = sizeof(client);

    getsockname(sockfd, (struct sockaddr *) &client, &client_size);
    port = ntohs(client.sin_port);
    return 0;
}