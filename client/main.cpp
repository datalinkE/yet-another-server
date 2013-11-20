#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <random>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

#include <google/protobuf/stubs/common.h>

#include "Message.pb.h"
#include "ServerStatus.pb.h"
#include "Echo.pb.h"
#include "Stub.pb.h"

using namespace std;

const size_t SIZE_BUFFER = 0x1000;
char buffer[SIZE_BUFFER];
const uint16_t PORT = 1972;
const char SERVER_ADDR[] = "127.0.0.1";

size_t NCHILDREN = 1;  // fork # child processes to call server
size_t NRUNS = 1;      // repeat child process function # times

// WARNING, with multiple chils console output could be massive
// redirect it to file for better experience

size_t TO_DO = 3;
// 0 - do nothing, accept() then close()
// 1 - status exchange
// 2 - echo exchange
// 3 - stub exchange (loads cpu by computing prime numbers)
// 4 - wrong data exchange
// 5 - even more dangerous wrong data
// >5 - random action from 0 to 5

size_t PRIMES_UNDER = 30000;
// parameter to mode 3

void child_func(int childnum);
int  get_socket();

void status_exchange(int sock, int childnum);
void echo_exchange(int sock, int childnum);
void stub_exchange(int sock, int childnum);

void do_exchange(yas::proto::Message & q, yas::proto::Message & ans_msg, int sock, int childnum);

void bad_exchange1(int sock, int childnum);
void bad_exchange2(int sock, int childnum);



int main(int argc, char *argv[])
{
    google::protobuf::LogSilencer silence; // comment this if you want protobuf debug messages at stderr

    int pid;
    int x;

    for (x = 0; x < NCHILDREN; x++) {
        if ((pid = fork()) == 0) {
            child_func(x + 1);
            exit(0);
        }
    }

    wait(NULL);
    sleep(1);
    return 0;
}

void child_func(int childnum)
{
    srand( unsigned( time(NULL) + childnum ) );

    for(size_t i = 0; i < NRUNS; i++)
    {

        int sock = get_socket();


        cout << "===" << endl;
        cout << "yas-client #" << childnum << " connected" << endl;

        if(TO_DO > 5)
            TO_DO= rand() % 6;

        try
        {
            switch(TO_DO)
            {
            case 0:
                // do nothing
                break;
            case 1:
                status_exchange(sock, childnum);
                break;
            case 2:
                echo_exchange(sock, childnum);
                break;
            case 3:
                stub_exchange(sock, childnum);
                break;
            case 4:
                bad_exchange1(sock, childnum);
                break;
            case 5:
                bad_exchange2(sock, childnum);
                break;
            }
        }
        catch(std::runtime_error e)
        {

            cout << "yas-client #" << childnum << " " << e.what() << endl;
        }

        close(sock);

        cout << "yas-client #" << childnum << " disconnected" << endl;
    }
}

int get_socket()
{
    int sock;
    struct sockaddr_in sAddr;
    int result;

    sAddr.sin_family = AF_INET;
    sAddr.sin_addr.s_addr = INADDR_ANY;
    sAddr.sin_port = 0;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(sock, (const struct sockaddr *) &sAddr, sizeof(sAddr));

    sAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    sAddr.sin_port = htons(PORT);

    result = connect(sock, (const struct sockaddr *) &sAddr, sizeof(sAddr));
    if (result != 0) {
        cerr << "yas-client fals to connect with error " << errno << ::endl;
        exit(errno);
    }

    return sock;
}

void status_exchange(int sock, int childnum)
{
    cout << "yas-client #" << childnum << " status_exchange" << endl;
    yas::proto::Message q;
    q.set_id("ServerStatus");

    yas::proto::Message ans_msg;

    do_exchange(q, ans_msg, sock, childnum);

    cout << "\t" << ans_msg.id() << endl;
    yas::proto::ServerStatusAnswer ans;
    ans.ParseFromString(ans_msg.data());

    cout << "\t"  << "version: " << ans.version() << endl;
    cout << "\t"  << "message processors: ";
    for(int i = 0 ; i < ans.messageprocessors_size(); i ++)
    {
        cout << ans.messageprocessors(i) << " ";
    }
    cout << endl;
}

void echo_exchange(int sock, int childnum)
{
    cout << "yas-client #" << childnum << " echo_exchange" << endl;
    yas::proto::Message q;
    yas::proto::EchoQuery echoQuery;
    echoQuery.set_msg("hello !");

    q.set_id("Echo");
    q.set_data(echoQuery.SerializePartialAsString());

    yas::proto::Message ans_msg;

    do_exchange(q, ans_msg, sock, childnum);

    cout << "\t"  << ans_msg.id() << endl;

    yas::proto::EchoAnswer ans;
    ans.ParseFromString(ans_msg.data());
    cout << "\t" << "server replies: " << ans.echo() << endl;

}

void stub_exchange(int sock, int childnum)
{
    cout << "yas-client #" << childnum << " stub_exchange" << endl;
    yas::proto::Message q;
    q.set_id("Stub");

    yas::proto::StubQuery stubQuery;
    stubQuery.set_seed(PRIMES_UNDER);
    q.set_data(stubQuery.SerializeAsString());

    yas::proto::Message ans_msg;

    do_exchange(q, ans_msg, sock, childnum);

    cout << "\t"  << ans_msg.id() << endl;

    yas::proto::StubAnswer ans;
    ans.ParseFromString(ans_msg.data());


    cout << "\t" << "server answered "<< endl;
    if(ans.has_comment())
    {
        cout << "\t" << "comment: " << ans.comment() << endl;
    }
}

void do_exchange(yas::proto::Message &q, yas::proto::Message &ans_msg, int sock, int childnum)
{
    const bool SLOW = false;//true;
    int bytes_sent, bytes_recieved;
    string serialQ= q.SerializeAsString();
    size_t serialSize = serialQ.size();

    if(serialSize == 0)
    {
        throw std::runtime_error("can't serialize query");
    }

    size_t serialSizeNet = htonl(serialSize);

    bytes_sent = send(sock, &serialSizeNet, sizeof(size_t), 0);
    if(SLOW) sleep(1);
    cout << "yas-client #" << childnum
           <<" sent "<< bytes_sent << " (message size)" << ::endl;

    bytes_sent = send(sock, serialQ.data() , serialSize, 0);
    if(SLOW) sleep(1);
    cout << "yas-client #" << childnum
           <<" sent "<< bytes_sent << " (message)" << ::endl;

    bytes_recieved = recv(sock, &serialSizeNet, sizeof(size_t), 0);
    if(SLOW) sleep(1);
    cout << "yas-client #" << childnum
           <<" recieved "<< bytes_recieved << " bytes(message size)" << ::endl;
    serialSize = ntohl(serialSizeNet);

    if(bytes_recieved <= 0 || serialSizeNet == 0)
    {
        throw std::runtime_error("no answer");
    }

    bytes_recieved = recv(sock, buffer, serialSize, 0);
    if(SLOW) sleep(1);
    cout << "yas-client #" << childnum
           <<" recieved "<< bytes_recieved << " bytes(message)" << ::endl;

    if(! ans_msg.ParseFromArray(buffer, serialSize))
    {
        throw std::runtime_error("can't deserealize answer");
    }
}

void bad_exchange1(int sock, int childnum)
{
    cout << "yas-client #" << childnum << " bad_exchange1" << endl;
    yas::proto::Message q;
    // wrong id field set
    q.set_id("nothing");

    yas::proto::Message ans_msg;

    try
    {
        do_exchange(q, ans_msg, sock, childnum);
    }
    catch(std::runtime_error e)
    {
        cout << "yas-client #" << childnum << " " << e.what() << endl;
        return;
    }

    yas::proto::StubAnswer ans;
    ans.ParseFromString(ans_msg.data());


    cout << "\t" << "server answered "<< endl;
    if(ans.has_comment())
    {
        cout << "\t" << "comment: " << ans.comment() << endl;
    }
}

void bad_exchange2(int sock, int childnum)
{
    cout << "yas-client #" << childnum << " bad_exchange2" << endl;
    // destructive... wrong size can ruin the server

    vector<int> buf = { 0, 10, 2, 30, 4, 50, 6, 70, 8, 90};

    random_shuffle(buf.begin(), buf.end());

    if( rand() > (RAND_MAX / 2) )
        buf[0] = htonl(buf[0]);

    int bytes_sent = send(sock, buf.data(), buf.size() * sizeof(int), 0);
    if(bytes_sent < 0)
    {
        cout << "yas-client #" << childnum << " error " << errno << endl;
        return;
    }
}
