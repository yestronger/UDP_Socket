#pragma once

//日志
#include<iostream>
#include<fstream>
#include<cstdio>
#include<string>
#include<ctime>
#include<unistd.h>
#include<sys/types.h>
#include<stdarg.h>
#include<pthread.h>
#include"LockGuard.hpp"

using namespace std;

bool gIsSave=false;
const string logname="log.txt";


void SaveFile(const string& filename,const string& message)
{
    ofstream out(filename,ios::app);
    if(!out.is_open())
    {
        return;
    }
    out<<message;
    out.close();
}

//1.日志是有等级的
enum Level
{
    DEBUG=0,
    INFO,
    WARNING,
    ERROR,
    FATAL
};


string LevelToString(int level)
{
    switch(level)
    {
        case DEBUG: return "Debug";break;
        case INFO: return "Info";break;
        case WARNING: return "Warning";break;
        case ERROR: return "Error";break;
        case FATAL: return "Fatal";break;
        default: return "Unknown";break;
    }
}

string GetTimeString()
{
    time_t curr_time=time(nullptr);
    struct tm* format_time=localtime(&curr_time);
    if(format_time==nullptr) return "None";
    char time_buffer[64];
    snprintf(time_buffer,sizeof(time_buffer),"%d-%d-%d %d:%d:%d",
    format_time->tm_year+1900,format_time->tm_mon+1,format_time->tm_mday,
    format_time->tm_hour,format_time->tm_min,format_time->tm_sec);

    return time_buffer;
}

pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;

//2.日志是由格式的
// 日志等级 时间 代码所在的文件名/行数 日志的内容...
void LogMessage(string filename,int line,bool issave,int level,const char* format,...)
{
    string levelstr=LevelToString(level);
    string timestr=GetTimeString();
    pid_t selfid=getpid();

    //可变参数部分处理
    char buffer[1024];
    va_list arg;
    va_start(arg,format);
    vsnprintf(buffer,sizeof(buffer),format,arg);
    va_end(arg);

    LockGuard lockguard(&lock);

    string message;
    message="["+timestr+"]"+"["+levelstr+"]"+"[pid: "
            +to_string(selfid)+"]"+"["+filename+"]"
            +"["+to_string(line)+"]"+buffer+"\n";
    if(!issave)
    {
        cout<<message;
    }
    else
    {
        SaveFile(logname,message);
    }
}




void Test(int num,...)
{
    va_list arg;
    va_start(arg,num);

    while(true)
    {
        int data=va_arg(arg,int);
        cout<<"data: "<<data<<endl;
        num--;
    }

    va_end(arg);//arg==NULL
}



//C99新特性 __VA_ARGS__
#define LOG(level,format,...) do {LogMessage(__FILE__,__LINE__,gIsSave,level,format,##__VA_ARGS__);} while(0)
#define EnableFile() do {gIsSave=true;} while(0)
#define EnableScreen() do {gIsSave=false;} while(0)


