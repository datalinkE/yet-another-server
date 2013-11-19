#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include <string>
#include <vector>
#include "Message.pb.h"
#include "ServerStatus.pb.h"

using namespace std;

void child_func(int childnum);

int main(int argc, char *argv[])
{
    int nchildren = 2;
    int pid;
    int x;

    for (x = 0; x < nchildren; x++) {
        if ((pid = fork()) == 0) {
            child_func(x + 1);
            exit(0);
        }
    }

    wait(NULL);
    sleep(1);
    return 0;
}

const size_t SIZE_BUFFER = 0x1000;

void child_func(int childnum)
{
    int sock;
    struct sockaddr_in sAddr;
    char buffer[SIZE_BUFFER];
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
        cerr << "yas-client " << result << ::endl;
      return;
    }

    yas::proto::Message q;
    q.set_id("ServerStatus");

    string serialQ= q.SerializeAsString();
    size_t serialSize = serialQ.size();

    bytes_sent = send(sock, &serialSize, sizeof(size_t), 0);
    sleep(1);
    cout << "yas-client #" << childnum
           <<" sent "<< bytes_sent << " (message size)" << ::endl;

    bytes_sent = send(sock, serialQ.data() , serialSize, 0);
    sleep(1);
    cout << "yas-client #" << childnum
           <<" sent "<< bytes_sent << " (message)" << ::endl;

    bytes_recieved = recv(sock, &serialSize, sizeof(size_t), 0);
    sleep(1);
    cout << "yas-client #" << childnum
           <<" recieved "<< bytes_recieved << " bytes(message size)" << ::endl;

    bytes_recieved = recv(sock, buffer, serialSize, 0);
    sleep(1);
    cout << "yas-client #" << childnum
           <<" recieved "<< bytes_recieved << " bytes(message)" << ::endl;

    yas::proto::Message ans_msg;
    ans_msg.ParseFromArray(buffer, serialSize);

    cout << ans_msg.id() << endl;
    yas::proto::ServerStatusAnswer ans;
    ans.ParseFromString(ans_msg.data());

    cout << "version: " << ans.version() << endl;
    cout << "message processors: ";
    for(int i = 0 ; i < ans.messageprocessors_size(); i ++)
    {
        cout << ans.messageprocessors(i) << " ";
    }
    cout << endl;
    close(sock);
}


