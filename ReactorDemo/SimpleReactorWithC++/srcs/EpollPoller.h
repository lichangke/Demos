//
// Created by leacock on 2020/11/16.
//

#ifndef SIMPLEREACTORWITHC___EPOLLPOLLER_H
#define SIMPLEREACTORWITHC___EPOLLPOLLER_H

#include <vector>
#include <sys/epoll.h>
#include <memory>
#include <sys/epoll.h>
#include <unistd.h>
#include <cassert>

#include "Handler.h"

/**
 * 事件分离器 封装了一个epoll进去
 */
class EpollPoller
{
    typedef std::vector<epoll_event> EventList; /// epoll_event
public:
    EpollPoller();
    ~EpollPoller();
    /// 添加 或者 更新 Handler 到  epoll 中
    void update(Handler* handler);
    /// 从 epoll 中 移除 handler
    void remove(Handler* handler);
    /// 调用 epoll_wait 等待 内核事件分发
    std::vector<Handler*> poll();

    static const int kInitEventListSize = 16;
private:
    std::vector<Handler*> fillActiveChannels(int activeNums);
private:
    int epfd_;  /// epoll 实例
    EventList activeEventList_;  /// 激活的事件列表
};



#endif //SIMPLEREACTORWITHC___EPOLLPOLLER_H
