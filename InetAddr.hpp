#include<iostream>
#include<string>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

using namespace std;

class InetAddr
{
private:
    void GetAddress(string* ip,uint16_t* port)
    {
        *port=ntohs(_addr.sin_port);//网络字节序转为主机字节序
        *ip=inet_ntoa(_addr.sin_addr);//将网络字节序IP转为点分式十进制IP
    }
public:
    InetAddr(const struct sockaddr_in &addr)
    :_addr(addr)
    {
        GetAddress(&_ip,&_port);
    }

    string Ip()
    {
        return _ip;
    }

    uint16_t Port()
    {
        return _port;
    }

    ~InetAddr()
    {}
private:
    struct sockaddr_in _addr;
    string _ip;
    uint16_t _port;
};