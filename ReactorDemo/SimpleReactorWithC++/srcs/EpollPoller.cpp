//
// Created by leacock on 2020/11/16.
//

#include <strings.h>
#include "EpollPoller.h"


EpollPoller::EpollPoller()
        : activeEventList_(kInitEventListSize) {
    epfd_ = epoll_create1(EPOLL_CLOEXEC);
}

EpollPoller::~EpollPoller() {
    close(epfd_);
}

/**
 * 添加 或者 更新 Handler 到  epoll 中
 * @param handler
 */
void EpollPoller::update(Handler* handler) {
    struct epoll_event epEvent{};
    bzero(&epEvent, sizeof epEvent);
    epEvent.events = handler->events(); /// 事件类型
    epEvent.data.ptr = handler; /// 抽象的事件描述符
    int op;
    if(handler->isInEpoll) /// 是否添加到 epoll 中
    {
        op = EPOLL_CTL_MOD;
    }
    else{
        op = EPOLL_CTL_ADD;
    }
    handler->isInEpoll = true;
    int ret = epoll_ctl(epfd_, op, handler->fd(), &epEvent);
    if(ret < 0){
        perror("epoll_ctl ADD");
        printf("epoll_ctl error on fd = %d\n", handler->fd());
    }
    else{
       /// printf("epoll_ctl on fd = %d success\n", handler->fd());
        printf("epoll_ctl add [fd = %d] events[%0x] success\n",handler->fd(), handler->events());
    }
}

/**
 * 从 epoll 中 移除 handler
 * @param handler
 */
void EpollPoller::remove(Handler* handler)
{
    assert(handler->isInEpoll);
    handler->isInEpoll = false;
    int op = EPOLL_CTL_DEL;
    int ret = epoll_ctl(epfd_, op, handler->fd(), NULL);
    if(ret < 0){
        perror("epoll_ctl DEL");
        printf("epoll_ctl error on fd = %d\n", handler->fd());
    }
    handler->isInEpoll = false;
}

/**
 * 对 分发的事件 设置对应的 revents_ 到 表明 哪类事件 触发了
 * @param activeNums
 * @return
 */
std::vector<Handler*> EpollPoller::fillActiveChannels(int activeNums)
{
    std::vector<Handler*> activeEventPtrList;
    for(int i = 0; i < activeNums; ++i)
    {
        /// activeEventList_ epoll_wait 的传出参数 哪些事件 被触发了
        auto *handlerPtr = static_cast<Handler*>(activeEventList_[i].data.ptr);
        if(activeEventList_[i].events & EPOLLIN)
        {
            handlerPtr->revents_ = EPOLLIN; /// 设置 revents_ 表明 哪类事件 触发了
        }
        if(activeEventList_[i].events & EPOLLOUT)
        {
            handlerPtr->revents_ = EPOLLOUT;
        }
        activeEventPtrList.push_back(handlerPtr);
    }
    return activeEventPtrList;
}

/**
 * 调用 epoll_wait 等待 内核事件分发
 * @return  分发的事件 list，  分发的事件中已设置对应的事件类型
 */
std::vector<Handler*> EpollPoller::poll() {
    std::vector<Handler*> activeEventPtrList;
    /// nreadys  分发的事件个数
    /// activeEventList_ 传出参数，表示发生的事件

    int nreadys = epoll_wait(epfd_, activeEventList_.data(), activeEventList_.size(), -1);
    if(nreadys > 0)
    {
        // 这里需要将就绪的eventList传递到Reactor中 通过函数返回
        /// 对 分发的事件 设置对应的 revents_  表明 哪类事件 触发了
        activeEventPtrList = fillActiveChannels(nreadys);
        if(nreadys == activeEventList_.size())
        {
            /// 扩容
            activeEventList_.resize(activeEventList_.size() * 2);
        }
    }
    else if(nreadys == 0)
    {
        // nothing happend
    }
    else{
        printf("nreadys = %d, error\n", nreadys);
    }
    return activeEventPtrList;
}
