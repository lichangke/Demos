//
// Created by leacock on 2020/11/16.
//

#ifndef SIMPLEREACTORWITHC___UPPERCHARACTERSERVER_H
#define SIMPLEREACTORWITHC___UPPERCHARACTERSERVER_H

#include "Reactor.h"
#include "Handler.h"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <functional>

#define my_error(x, str) \
do{\
    if(x < 0)\
    {   perror(str); exit(-1);}\
}while(0)

using namespace std::placeholders;

class UpperCharacterServer
{
public:
    explicit UpperCharacterServer(int port);
    ~UpperCharacterServer();

    void start();

    void startup();

private:
    /// accept 回调函数
    void accpetConn(int fd);
    /// read 回调函数
    void readdata(int fd);
    /// send 回调函数
    void senddata(int fd);

    char convert_char(char c);

private:
    int port_; /// 服务端口
    std::string ip_; /// ip地址
    std::unique_ptr<Reactor> base_; /// Reactor 实例
    //Reactor* base_; /// Reactor 实例
    std::map<int, std::shared_ptr<Handler>> handleList_;  /// handle 字典  ， key 为描述符
    //std::map<int, Handler*> handleList_;  /// handle 字典  ， key 为描述符
};


#endif //SIMPLEREACTORWITHC___UPPERCHARACTERSERVER_H
