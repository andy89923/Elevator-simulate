#include <arpa/inet.h>
#include <sys/socket.h>

#include <cstring>
#include <iostream>
using namespace std;

#define MAXCONN 10

void tcp_server(int& sock, string listen_ip, int port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Error on socket init!\n";
        exit(1);
    }

    const int opt = true;
    socklen_t optlen = sizeof(opt);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, optlen);
    setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &opt, optlen);

    sockaddr_in server_id;
    // bzero(&server_id, sizeof(server_id));

    server_id.sin_family = PF_INET;
    server_id.sin_port = htons(port);
    // server_id.sin_addr.s_addr = htonl(INADDR_ANY);
    server_id.sin_addr.s_addr = inet_addr(listen_ip.c_str());

    int r = bind(sock, (sockaddr*)&server_id, sizeof(sockaddr_in));
    if (r < 0) {
        cerr << "Error on bind!\n";
        exit(1);
    }
    r = listen(sock, 10);
}

void tcp_client(int& sock, string server_ip, int port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Error on socket init!\n";
        exit(1);
    }
    sockaddr_in server_id;
    server_id.sin_family = AF_INET;
    server_id.sin_port = htons(port);
    server_id.sin_addr.s_addr = inet_addr(server_ip.c_str());

    int err = connect(sock, (struct sockaddr*)&server_id, sizeof(server_id));
    if (err < 0) {
        cerr << "Error on connect to server!\n";
        exit(1);
    }
}

void new_client_info(char* client_ip, int port, string endStr = "\n") {
    cout << "New connection from " << client_ip << ":" << port << endStr;
}

int acceptClient(int socket) {
    int client_fd;
    sockaddr_in client_id;
    int info_len = sizeof(sockaddr);
    memset(&client_id, 0, sizeof(client_id));

    for (;;) {
        if ((client_fd = accept(socket, (sockaddr*)&client_id,
                                (socklen_t*)&info_len)) < 0) {
            if (errno == EINTR)
                continue;
            else {
                cerr << "Connect error!\n";
                exit(1);
            }
        }
        new_client_info(inet_ntoa(client_id.sin_addr),
                        ntohs(client_id.sin_port));
        return client_fd;
    }
    return -1;
}
