#include <arpa/inet.h>
#include <sys/socket.h>
using namespace std;

void tcp_socket(int& sock, sockaddr_in& server_id, int port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        cout << "Error on socket init!\n";
        exit(1);
    }
    const int opt = true;
    socklen_t optlen = sizeof(opt);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, optlen);
    setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &opt, optlen);

    bzero(&server_id, sizeof(server_id));

    server_id.sin_family = PF_INET;
    server_id.sin_port = htons(port);
    server_id.sin_addr.s_addr = htonl(INADDR_ANY);

    int r = bind(sock, (sockaddr*)&server_id, sizeof(sockaddr_in));
    if (r < 0) {
        cerr << "Error on bind!\n";
        exit(1);
    }
    r = listen(sock, MAXCONN);
}