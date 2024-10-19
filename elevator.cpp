#include <signal.h>
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

    void thread_print_state() {
        while (this->running) {
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
            cout << "Status: " << state_str << ", " << "Floor: " << state.floor
                 << "\n";
        }
    }

    void thread_simulate() {
        while (this->running) {
            this_thread::sleep_for(chrono::seconds(1));  // remove later
        }
    }

    void transitState(int time, State nextState) {
        this_thread::sleep_for(chrono::seconds(time));
        {
            lock_guard<mutex> lock(state_mutex);
            state = nextState;
        }
    }

    void thread_receiveCommand(int controller_sock) {
        for (;;) {
            int read_len = read(controller_sock, buf, BUFFERSIZ);
            if (read_len == 0) {
                return;
            }

            int command = buf[0] - '0';

            // TODO
            cout << "receive command: " << command << '\n';
        }
    }

   public:
    bool running = false;
    thread printThread, elevatorThread;
    Elevator() : state(Close, 1) {}

    void run() {
        running = true;
        printThread = thread(&Elevator::thread_print_state, this);
        elevatorThread = thread(&Elevator::thread_simulate, this);
    }

    void stop() {
        running = false;
        cout << "Stopping elevator...\n";

        printThread.join();
        elevatorThread.join();

        cout << "Elevator stopped!\n";
    }

    void addController(int sock) {
        thread(&Elevator::thread_receiveCommand, this, sock).detach();
    }
};

Elevator elevator;
void sig_term(int signo) {
    cout << "Terminating...\n";
    elevator.stop();

    exit(0);
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
    signal(SIGINT, sig_term);

    int socket;

    cout << "Openning server on " << server_ip << ":" << server_port << "\n";
    tcp_server(socket, server_ip, server_port);

    printElevatorMsg();

    elevator.run();
    for (;;) {
        int client_fd = acceptClient(socket);
        elevator.addController(client_fd);
    }
    return 0;
}