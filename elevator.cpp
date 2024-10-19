#include <unistd.h>

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

#include "utils/socket.cpp"

using namespace std;

void printElevatorMsg() {
    cout << "+----------------------+\n";
    cout << "|       Elevator       |\n";
    cout << "+----------------------+\n";
}

#define BUFFERSIZ 10
char buf[BUFFERSIZ];

enum Stauts { Close, Open, Move };

struct State {
    Stauts status;
    int floor;

    State(Stauts status, int floor) {
        this->status = status;
        this->floor = floor;
    }

    bool operator==(const State& rhs) {
        return (status == rhs.status && floor == rhs.floor);
    }
};

class Elevator {
   protected:
    State state;
    mutex state_mutex;

    void print_state() {
        while (true) {
            this_thread::sleep_for(std::chrono::seconds(1));
            string state_str;
            {
                lock_guard<mutex> lock(state_mutex);
                switch (state.status) {
                    case Close:
                        state_str = "Close";
                        break;
                    case Open:
                        state_str = "Open";
                        break;
                    case Move:
                        state_str = "Move";
                        break;
                }
            }
            cout << "Elevator status: " << state_str
                 << ", Current floor: " << state.floor << "\n";
        }
    }

   public:
    Elevator() : state(Close, 1) {}

    void run() { thread(&Elevator::print_state, this).detach(); }
};

void receiveCommand(int& controller_sock) {
    for (;;) {
        int read_len = read(controller_sock, buf, BUFFERSIZ);
        if (read_len == 0) {
            return;
        }

        // TODO
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

    Elevator elevator;
    printElevatorMsg();

    elevator.run();

    for (;;) {
        int client_fd = acceptClient(socket);

        thread(receiveCommand, client_fd);
    }
    return 0;
}