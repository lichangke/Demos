//
// Created by leacock on 2020/11/16.
//

#ifndef SIMPLEREACTORWITHC___REACTOR_H
#define SIMPLEREACTORWITHC___REACTOR_H

#include <memory>

#include "EpollPoller.h"
#include "Handler.h"

/**
 * reactor 负责将 handler 注册到 demultiplexer 上 这里是 EpollPoller。
 */
class Reactor
{
public:
    Reactor();
    ~Reactor();
    /// 注册  Handler 到 EpollPoller
    void registerHandler(Handler* );
    /// 从 EpollPoller 移除 Handler
    void removeHandler(Handler*);

    void loop();
private:
    std::unique_ptr<EpollPoller> poller_;
};


#endif //SIMPLEREACTORWITHC___REACTOR_H
