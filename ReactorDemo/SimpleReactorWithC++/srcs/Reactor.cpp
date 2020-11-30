//
// Created by leacock on 2020/11/16.
//

#include "Reactor.h"

Reactor::Reactor() {
    poller_ = static_cast<std::unique_ptr<EpollPoller>>(new EpollPoller);
}

Reactor::~Reactor() = default;

/**
 * 注册  Handler 到 EpollPoller
 * @param handler    handler 抽象事件
 */
void Reactor::registerHandler(Handler* handler) {
    poller_->update(handler);
}
/**
 * 从 EpollPoller 移除 Handler
 * @param handler  handler 抽象事件
 */
void Reactor::removeHandler(Handler* handler) {
    poller_->remove(handler);
}

/**
 * Synchronous Event Demultiplexer 同步（多路）事件分离器，
 */
void Reactor::loop() {
    while(true)
    {
        /// 调用 epoll_wait 等待内核 I/O 事件的分发， 返回 分发的事件 list
        auto activeEventPtrList = poller_->poll();

        if(!activeEventPtrList.empty())
        {
            /// 遍历处理事件
            for(int i = 0; i < activeEventPtrList.size(); ++i)
            {
                /// 执行事件  调用回调函数
                activeEventPtrList[i]->handleEvent();
            }
        }
        else{
            sleep(0.001);
        }
    }
}




