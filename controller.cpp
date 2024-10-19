#include <iostream>

#include "utils/socket.cpp"

using namespace std;

static void printControllerMsg() {
    cout << "+----------------------+\n";
    cout << "|      Controller      |\n";
    cout << "|----------------------|\n";
    cout << "| 1 - 1F (Inside)      |\n";
    cout << "| 2 - 2F (Inside)      |\n";
    cout << "| 3 - 1F Up            |\n";
    cout << "| 4 - 2F Down          |\n";
    cout << "| 0 - Controller Exit  |\n";
    cout << "+----------------------+\n";
}

int main(int argc, char* argv[]) {
    string server_ip = "127.0.0.1";
    int server_port = 12345;

    if (argc < 3) {
        cout << "No server specify, use default server IP & Port.\n";
    } else {
        server_ip = argv[1];
        server_port = atoi(argv[2]);
    }

    int socket;

    cout << "Connecting to " << server_ip << ":" << server_port << " - ";
    tcp_client(socket, server_ip, server_port);
    cout << "OK!\n\n";

    printControllerMsg();

    char buf[5];
    while (true) {
        memset(buf, 0, sizeof(buf));
        int command;

        cin >> command;
        if (command == 0) {
            cout << "Controller Exit!\n";
            return 0;
        }
        if (command < 0 || command > 4) {
            cout << "Invalid command: " << command << "\n";
            continue;
        }

        buf[0] = '0' + command;
        buf[1] = 0;

        int send_size = send(socket, buf, 2, 0);
        if (send_size == 2) {
            cout << "Command " << command << " sent\n";
        }
    }
    return 0;
}