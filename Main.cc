#include<iostream>
#include<memory>
#include"UdpServer.hpp"
#include"Log.hpp"
using namespace std;

void Usage(string proc)
{
    cout<<"Usage:\n\t"<<proc<<" local_port\n"<<endl;
}

// ./udpserver port
int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        Usage(argv[0]);
        exit(USAGE_ERROR);
    }
    EnableScreen();
    //string ip=argv[1];
    uint16_t port=stoi(argv[1]);
    unique_ptr<UdpServer> usvr=make_unique<UdpServer>(port);
    usvr->InitServer();
    usvr->Start();

    return 0;
}