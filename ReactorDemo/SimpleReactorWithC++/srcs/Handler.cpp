//
// Created by leacock on 2020/11/16.
//

#include "Handler.h"

#include <utility>


Handler::Handler(int fd)
        : fd_(fd)
        , events_(0)
        , revents_(0)
        , isInEpoll(false)
        , len_(0) {

}

Handler::~Handler() {
    // close(fd)
    // Handler 并不负责关闭fd，fd的关闭应该在连接断开的时候
}

/**
 * 执行事件  调用回调函数
 */
void Handler::handleEvent() {
    if(revents_ & EPOLLIN)
    {
        readCallback_(fd_);
    }
    if(revents_ & EPOLLOUT)
    {
        writeCallback_(fd_);
    }
}

/**
 * 设置 read 回调函数
 * @param cb  回调函数
 */
void Handler::setReadCallback(EventCallback cb) {
    readCallback_ = std::move(cb);
}

/**
 * 设置 write 回调函数
 * @param cb  回调函数
 */
void Handler::setWriteCallback(EventCallback cb) {
    writeCallback_ = std::move(cb);
}

/**
 * 设置 close 回调函数
 * @param cb
 */
void Handler::setCloseCallback(EventCallback cb) {
    closeCallback_ = cb;
}

/**
 * 使能 EPOLLIN 事件类型
 */
void Handler::enableRead() {
    events_ = EPOLLIN;
}

/**
 * 使能 EPOLLOUT 事件类型
 */
void Handler::enableWrite() {
    events_ = EPOLLOUT;
}

/**
 * 使能 EPOLLOUT  EPOLLOUT 事件类型
 */
void Handler::enableAll() {
    events_ |= EPOLLIN;
    events_ |= EPOLLOUT;
}


void Handler::disableAll(){
    events_ = 0;
}
