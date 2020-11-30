//
// Created by leacock on 2020/11/16.
//

#ifndef SIMPLEREACTORWITHC___HANDLER_H
#define SIMPLEREACTORWITHC___HANDLER_H

#include <iostream>
#include <sys/epoll.h>
#include "Callback.h"


#define MAX_BUFFLEN 1024

/*
handler 抽象的事件描述符
设置fd
设置关注的事件类型
设置回调函数
*/
class Handler
{
    friend class EpollPoller;
public:
    explicit Handler(int fd);
    ~Handler();
    void handleEvent(); /// 执行事件  调用回调函数
    void setReadCallback(EventCallback);
    void setWriteCallback(EventCallback);
    void setCloseCallback(EventCallback);
    void enableRead();
    void enableWrite();
    void enableAll();
    void disableAll();
    int fd() const{
        return fd_;
    }
    int events() const
    {
        return events_;
    }
    int revents() const
    {
        return revents_;
    }
    char *buff()
    {
        return buff_;
    }
    void setLen(int len)
    {
        len_ = len;
    }
    int length()
    {
        return len_;
    }

    bool isInEpoll;
    int revents_;


private:
    int fd_; /// 描述符
    int events_; /// 事件类型
    /// 是否还在epoll的监控中

    int len_; /// 缓冲区使用的大小，也就是字符串实际长度 包含结束符
    char buff_[MAX_BUFFLEN]{}; /// 一个缓冲区
    EventCallback readCallback_;  /// read 回调函数
    EventCallback writeCallback_; /// write 回调函数
    EventCallback closeCallback_;
    EventCallback errorCallback_;
};



#endif //SIMPLEREACTORWITHC___HANDLER_H
