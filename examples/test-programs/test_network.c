/**
 * Test Case 12: Network Programming
 * Tests: Socket creation, TCP/UDP, client-server, socket options
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define BACKLOG 5

// Test socket creation
void test_socket_creation() {
    printf("\n1. Socket Creation:\n");
    
    // TCP socket
    int tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_sock < 0) {
        perror("TCP socket creation failed");
    } else {
        printf("TCP socket created: fd=%d\n", tcp_sock);
        close(tcp_sock);
    }
    
    // UDP socket
    int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock < 0) {
        perror("UDP socket creation failed");
    } else {
        printf("UDP socket created: fd=%d\n", udp_sock);
        close(udp_sock);
    }
}

// Test socket options
void test_socket_options() {
    printf("\n2. Socket Options:\n");
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return;
    }
    
    // Set SO_REUSEADDR
    int reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt SO_REUSEADDR");
    } else {
        printf("SO_REUSEADDR enabled\n");
    }
    
    // Set SO_KEEPALIVE
    int keepalive = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive)) < 0) {
        perror("setsockopt SO_KEEPALIVE");
    } else {
        printf("SO_KEEPALIVE enabled\n");
    }
    
    // Get socket type
    int sock_type;
    socklen_t opt_len = sizeof(sock_type);
    if (getsockopt(sock, SOL_SOCKET, SO_TYPE, &sock_type, &opt_len) < 0) {
        perror("getsockopt SO_TYPE");
    } else {
        printf("Socket type: %s\n", 
               sock_type == SOCK_STREAM ? "SOCK_STREAM" : "SOCK_DGRAM");
    }
    
    close(sock);
}

// Test address structures
void test_address_structures() {
    printf("\n3. Address Structures:\n");
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    printf("Address family: %d\n", addr.sin_family);
    printf("Port (network): %u\n", addr.sin_port);
    printf("Port (host): %u\n", ntohs(addr.sin_port));
    
    // inet_pton - presentation to network
    struct in_addr ip_addr;
    if (inet_pton(AF_INET, "127.0.0.1", &ip_addr) == 1) {
        printf("inet_pton: 127.0.0.1 converted successfully\n");
        
        // inet_ntop - network to presentation
        char ip_str[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &ip_addr, ip_str, INET_ADDRSTRLEN) != NULL) {
            printf("inet_ntop: %s\n", ip_str);
        }
    }
}

// Test hostname resolution
void test_hostname_resolution() {
    printf("\n4. Hostname Resolution:\n");
    
    struct hostent *host = gethostbyname("localhost");
    if (host == NULL) {
        herror("gethostbyname");
        return;
    }
    
    printf("Official name: %s\n", host->h_name);
    printf("Address type: %s\n", 
           host->h_addrtype == AF_INET ? "AF_INET" : "Unknown");
    printf("Address length: %d\n", host->h_length);
    
    // Print IP addresses
    printf("Addresses:\n");
    for (int i = 0; host->h_addr_list[i] != NULL; i++) {
        struct in_addr addr;
        memcpy(&addr, host->h_addr_list[i], sizeof(struct in_addr));
        printf("  %s\n", inet_ntoa(addr));
    }
}

// Test getaddrinfo (modern way)
void test_getaddrinfo() {
    printf("\n5. getaddrinfo (modern resolution):\n");
    
    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;
    
    int status = getaddrinfo("localhost", "80", &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return;
    }
    
    printf("Address info for localhost:80:\n");
    for (p = res; p != NULL; p = p->ai_next) {
        void *addr;
        char ipstr[INET6_ADDRSTRLEN];
        
        if (p->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            printf("  IPv4: ");
        } else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            printf("  IPv6: ");
        }
        
        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        printf("%s\n", ipstr);
    }
    
    freeaddrinfo(res);
}

// Test TCP server setup (bind and listen only)
void test_tcp_server_setup() {
    printf("\n6. TCP Server Setup (bind/listen):\n");
    
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return;
    }
    
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind");
        close(server_fd);
        return;
    }
    printf("Socket bound to port %d\n", PORT);
    
    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen");
        close(server_fd);
        return;
    }
    printf("Socket listening (backlog=%d)\n", BACKLOG);
    
    close(server_fd);
    printf("Server socket closed\n");
}

// Test UDP socket operations
void test_udp_operations() {
    printf("\n7. UDP Socket Operations:\n");
    
    int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock < 0) {
        perror("socket");
        return;
    }
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT + 1);
    
    if (bind(udp_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(udp_sock);
        return;
    }
    printf("UDP socket bound to port %d\n", PORT + 1);
    
    // Get socket name
    struct sockaddr_in bound_addr;
    socklen_t addr_len = sizeof(bound_addr);
    if (getsockname(udp_sock, (struct sockaddr *)&bound_addr, &addr_len) == 0) {
        printf("Bound to: %s:%d\n", 
               inet_ntoa(bound_addr.sin_addr),
               ntohs(bound_addr.sin_port));
    }
    
    close(udp_sock);
}

// Test socket pair (IPC)
void test_socketpair() {
    printf("\n8. Socket Pair (Unix domain sockets):\n");
    
    int sv[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) < 0) {
        perror("socketpair");
        return;
    }
    
    printf("Socket pair created: fd1=%d, fd2=%d\n", sv[0], sv[1]);
    
    // Write to one end
    const char *msg = "Hello from socket pair!";
    if (write(sv[0], msg, strlen(msg)) < 0) {
        perror("write");
    } else {
        printf("Sent: %s\n", msg);
    }
    
    // Read from other end
    char buffer[BUFFER_SIZE];
    ssize_t n = read(sv[1], buffer, sizeof(buffer) - 1);
    if (n > 0) {
        buffer[n] = '\0';
        printf("Received: %s\n", buffer);
    }
    
    close(sv[0]);
    close(sv[1]);
}

// Test select for I/O multiplexing
void test_select() {
    printf("\n9. select() for I/O multiplexing:\n");
    
    fd_set readfds;
    struct timeval tv;
    
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    
    printf("Waiting 2 seconds for stdin input...\n");
    int retval = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
    
    if (retval == -1) {
        perror("select");
    } else if (retval) {
        printf("Data is available on stdin\n");
    } else {
        printf("Timeout occurred (no data)\n");
    }
}

int main() {
    printf("=== Network Programming Test ===\n");
    
    test_socket_creation();
    test_socket_options();
    test_address_structures();
    test_hostname_resolution();
    test_getaddrinfo();
    test_tcp_server_setup();
    test_udp_operations();
    test_socketpair();
    test_select();
    
    printf("\n=== Network Programming Test Complete ===\n");
    printf("Note: Full client-server tests require actual connections\n");
    
    return 0;
}
