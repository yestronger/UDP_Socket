#pragma once
#include <iostream>
#include <string>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdlib.h>
#include "Log.hpp"
#include"InetAddr.hpp"

using namespace std;

enum
{
    SOCKET_ERROR = 1,
    BIND_ERROR,
    USAGE_ERROR
};

const static int defaultfd = -1;

class UdpServer
{
public:
    UdpServer(uint16_t port)
        : _sockfd(defaultfd), _port(port)
        ,_isrunning(false)
    {
    }

    void InitServer()
    {
        // 1.创建udp socket 套接字...必须要做的
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            LOG(FATAL, "socket error,%s,%d\n", strerror(errno), errno);
            exit(SOCKET_ERROR);
        }
        LOG(INFO, "socket create success,sockfd: %d\n", _sockfd);

        // 2.1 填充sockaddr_in结构
        struct sockaddr_in local;     // struct sockaddr_in 系统提供的数据类型，local是变量，用户栈上开辟空间
        bzero(&local, sizeof(local)); // 清空
        local.sin_family = AF_INET;
        local.sin_port = htons(_port); // port要经过网络传输给对面，即port先到网络，所以要将_port,从主机序列转化为网络序列
        //a.字符串风格的点分十进制的IP地址转成4字节IP
        //b.将主机序列转成网络序列
        //in_addr_t inet_addr(const char* cp)->同时完成a&b
        //local.sin_addr.s_addr =inet_addr(_ip.c_str());
        local.sin_addr.s_addr=INADDR_ANY;//htonl(INADDR_ANY)

        // 2.2 bind sockfd和网络信息(IP(?)+Port)
        int n = bind(_sockfd,(struct sockaddr*)&local,sizeof(local));
        if(n<0)
        {
            LOG(FATAL, "bind error,%s,%d\n", strerror(errno), errno);
            exit(BIND_ERROR);
        }
        LOG(INFO, "socket bind success\n");

    }

    void Start()
    {
        //一直运行，直到管理者不想运行了，服务器都是死循环
        _isrunning=true;
        while(true)
        {
            char buffer[1024];
            struct sockaddr_in peer;
            socklen_t len=sizeof(peer);
            //1.我们要让server先收数据
            ssize_t n=recvfrom(_sockfd,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&peer,&len);
            if(n>0)
            {
                buffer[n]=0;
                InetAddr addr(peer);
                LOG(DEBUG,"get message from [%s:%d]: %s\n",addr.Ip().c_str(),addr.Port(),buffer);
                //2.我们要将server收到的数据，发回给对方
                sendto(_sockfd,buffer,strlen(buffer),0,(struct sockaddr*)&peer,len);
            }

        }
        _isrunning=false;
    }

    ~UdpServer()
    {
    }

private:
    int _sockfd;
    //string _ip;//不是必须的
    uint16_t _port; // 服务器所用的端口号
    bool _isrunning;
};