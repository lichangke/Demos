//
// Created by leacock on 2020/11/16.
//

#include "UpperCharacterServer.h"

UpperCharacterServer::UpperCharacterServer(int port)
        : port_(port)
        , base_(new Reactor) {
}

UpperCharacterServer::~UpperCharacterServer() {
}


void UpperCharacterServer::start() {
    startup();
    base_->loop();
}

/**
 * 创建 服务端的监听套接字
 * 创建 设置 accpet 事件 到 epoll
 */
void UpperCharacterServer::startup() {

    /// 创建监听套接字
    int lfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    my_error(lfd, "socket error");

    /// 创建 accpet handler 实例
    auto lHandler = std::make_shared<Handler>(lfd);
//    auto lHandler = new Handler(lfd);
//    if (handleList_[lfd]!= nullptr){
//        delete(handleList_[lfd]);
//    }
    handleList_[lfd] = lHandler;

    int reuse = 1;
    setsockopt(lHandler->fd(), SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse);
    struct sockaddr_in addr{};
    bzero(&addr, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int ret = bind(lHandler->fd(), (sockaddr*)&addr, sizeof addr);
    my_error(ret, "bind error");

    ret = listen(lHandler->fd(), 16);
    my_error(ret, "listen error");

    /// 设置回调函数  accpet 事件的回调 accpetConn
    lHandler->setReadCallback(std::bind(&UpperCharacterServer::accpetConn, this, _1));
    /// 使能 read 事件类型
    lHandler->enableRead();
    /// 注册 描述符到 epoll 中
    base_->registerHandler(lHandler.get());
    //base_->registerHandler(lHandler);

}

/**
 * accept 事件 处理 回调函数
 * @param fd
 */
void UpperCharacterServer::accpetConn(int fd) {

    printf("in accept\n");
    /// 这种方式也可以设置 为 SOCK_NONBLOCK 非阻塞
    int cfd = accept4(fd, NULL, NULL, SOCK_NONBLOCK);
    my_error(cfd, "accept4");
    /// 创建 read handler 实例
    auto connHandler = std::make_shared<Handler>(cfd);
//    auto connHandler = new Handler(cfd);
//    if (handleList_[cfd]!= nullptr){
//        delete(handleList_[cfd]);
//    }
    handleList_[cfd] = connHandler;
    /// 设置回调函数  read 事件的回调 readdata
    connHandler->setReadCallback(std::bind(&UpperCharacterServer::readdata, this, _1));
    /// 使能 read 事件类型
    connHandler->enableRead();
    /// 注册 描述符到 epoll 中
    base_->registerHandler(connHandler.get());
    //base_->registerHandler(connHandler);

}

/**
 * read 事件 处理 回调函数
 * @param fd
 */
void UpperCharacterServer::readdata(int fd) {

    printf("in read\n");
    auto pHandler = handleList_[fd];
    bzero(pHandler->buff(),MAX_BUFFLEN);
    int nbytes = read(fd, pHandler->buff(), MAX_BUFFLEN);
    if(nbytes > 0)
    {
        /// TODO 这里len 可能 == MAX_BUFFLEN
        int len = strlen(pHandler->buff());
        for (int i = 0; i < len; ++i) {
            pHandler->buff()[i] = convert_char(pHandler->buff()[i]);
        }

        printf("%s\n", pHandler->buff());
        pHandler->setLen(len);
        /// 设置回调函数  send 事件的回调 senddata
        pHandler->setWriteCallback(std::bind(&UpperCharacterServer::senddata, this, _1));
        /// 使能 write 事件类型
        pHandler->enableWrite();
        /// 注册 描述符到 epoll 中
        base_->registerHandler(pHandler.get());
        //base_->registerHandler(pHandler);
    }
    else if(nbytes == 0){
        printf("close fd\n");
        close(fd);
        base_->removeHandler(pHandler.get());
        //base_->removeHandler(pHandler);
        handleList_.erase(fd);
    }
    else{
        close(fd);
        printf("read error\n");
        base_->removeHandler(pHandler.get());
        //base_->removeHandler(pHandler);
        handleList_.erase(fd);
    }

}

/**
 * send 事件 处理 回调函数
 * @param fd
 */
void UpperCharacterServer::senddata(int fd) {

    auto pHandler = handleList_[fd];
    int nbytes = write(fd, pHandler->buff(), pHandler->length());
    if(nbytes >= 0)
    {
        pHandler->setReadCallback(std::bind(&UpperCharacterServer::readdata, this, _1));
        pHandler->enableRead();
        base_->registerHandler(pHandler.get());
        //base_->registerHandler(pHandler);
    }
    else {
        printf("write error\n");
        close(fd);
        base_->removeHandler(pHandler.get());
        //base_->removeHandler(pHandler);
        handleList_.erase(fd);
    }

}

char UpperCharacterServer::convert_char(char c) {
    if ( 'A' <= c && c <= 'Z')
        return c + 32; // 转换小写
    else if ( 'a' <= c && c <= 'z')
        return c - 32; // 转换大写
    else
        return c; // 其他不变
}



