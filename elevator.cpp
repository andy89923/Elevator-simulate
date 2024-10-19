#include <unistd.h>

#include <iostream>

#include "utils/socket.cpp"

using namespace std;

void printElevatorMsg() {
    cout << "+----------------------+\n";
    cout << "|       Elevator       |\n";
    cout << "+----------------------+\n";
}

#define BUFFERSIZ 10
char buf[BUFFERSIZ];

void receiveCommand(int& controller_sock) {
    for (;;) {
        int read_len = read(controller_sock, buf, BUFFERSIZ);
        if (read_len == 0) {
            return;
        }

        cout << read_len << " : " << buf << '\n';
    }
}

int main(int argc, char* argv[]) {
    string server_ip = "127.0.0.1";
    int server_port = 12345;

    if (argc < 3) {
        cerr << "[Usage]\n";
        cerr << "./elevator <ListenIP> <ListenPort>\n\n";
        cout << "Use default server IP & Port!\n";
    } else {
        server_ip = argv[1];
        server_port = atoi(argv[2]);
    }

    int socket;

    cout << "Openning server on " << server_ip << ":" << server_port << "\n";
    tcp_server(socket, server_ip, server_port);

    printElevatorMsg();

    for (;;) {
        int client_fd = acceptClient(socket);

        receiveCommand(client_fd);
    }
    return 0;
}