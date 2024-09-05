#include<iostream>
#include<string>
#include<cstdio>
#include<cstring>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

using namespace std;


void Usage(string proc)
{
    cout<<"Usage:\n\t"<<proc<<" serverip serverport\n"<<endl;
}


// ./udpclient serverip serverport
int main(int argc,char *argv[])
{
    if(argc!=3)
    {
        Usage(argv[0]);
        exit(1);
    }
    string serverip=argv[1];
    uint16_t serverport=stoi(argv[2]);

    //1.创建socket
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0)
    {
        cerr<<"socket error"<<endl;
    }

    //2.client一定要bind，client也有自己的ip和port，但是不建议显示(和server一样用bind函数)bind
    //a.那如何bind呢？当udp client首次发送数据的时候，os会自动随机的给client进行bind--为什么？要bind，必然要和port关联！防止client port冲突
    //b.什么时候bind？首次发送数据的时候

    //构建目标主机的socket信息
    struct sockaddr_in server;
    memset(&server,0,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port=htons(serverport);
    server.sin_addr.s_addr=inet_addr(serverip.c_str());

    string message;
    //3.直接通信即可
    while(true)
    {
        cout<<"Please Enter# ";
        getline(cin,message);
        sendto(sockfd,message.c_str(),message.size(),0,(struct sockaddr*)&server,sizeof(server));
        
        struct sockaddr_in peer;
        socklen_t len=sizeof(peer);
        char buffer[1024];
        ssize_t n=recvfrom(sockfd,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&peer,&len);
        if(n>0)
        {
            buffer[n]=0;
            cout<<"server echo# "<<buffer<<endl;
        }
    }
    return 0;
}