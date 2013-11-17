#include <iostream>

using namespace std;

#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

void child_func(int childnum);

int main(int argc, char *argv[])
{
    int nchildren = 1;
    int pid;
    int x;

    for (x = 0; x < nchildren; x++) {
        if ((pid = fork()) == 0) {
            child_func(x + 1);
            exit(0);
        }
    }

    wait(NULL);

    return 0;
}

const size_t SIZE_BUFFER = 25;

void child_func(int childnum)
{
    int sock;
    struct sockaddr_in sAddr;
    char buffer[25];
    int x;
    int result;
    size_t bytes_sent, bytes_recieved;

    sAddr.sin_family = AF_INET;
    sAddr.sin_addr.s_addr = INADDR_ANY;
    sAddr.sin_port = 0;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(sock, (const struct sockaddr *) &sAddr, sizeof(sAddr));

    sAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sAddr.sin_port = htons(1972);

    result = connect(sock, (const struct sockaddr *) &sAddr, sizeof(sAddr));
    if (result != 0) {
        ::cerr << "yas-client " << result << ::endl;
      return;
    }

    snprintf(buffer, SIZE_BUFFER, "data from client #%i.", childnum);
    sleep(1);
    bytes_sent = send(sock, buffer, strlen(buffer), 0);
    ::cout << "yas-client #" << childnum
           <<" sent "<< bytes_sent << " chars" << ::endl;
    sleep(1);
    bytes_recieved = recv(sock, buffer, SIZE_BUFFER, 0);
    ::cout << "yas-client #" << childnum
           <<" recieved "<< bytes_recieved << " chars" << ::endl;
    sleep(1);

    close(sock);
}


