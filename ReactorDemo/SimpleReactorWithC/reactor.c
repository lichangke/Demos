

#include <sys/epoll.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>

#define MAX_EVENTS 1024
#define BUFFLEN 4096
#define my_error(x, str) \
do{\
    if(x < 0)\
    {   perror(str); exit(-1);}\
}while(0)

void recvdata(int fd, int events, void *arg);
void senddata(int fd, int events, void *arg);
typedef void (*Callback_t)(int , int , void*);

/// 抽象事件
struct myevent_s{
    int fd; // 这里是 连接 socket
    int events; // 记录所关怀的事件类型，读事件还是写事件
    void *arg; // 指向自己的指针
    Callback_t call_back;  // 回调函数
    int status;  // 记录自己是否在epoll树上  1 在epoll树上
    int index;
    char buf[BUFFLEN];  // 一个缓冲区
    int len;  // 缓冲区有效数据的长度
};

int g_efd;
struct myevent_s g_events[MAX_EVENTS+1];

void eventReset(struct myevent_s *ev)
{
    bzero(ev->buf, sizeof ev->buf);
    ev->len = 0;
}

char convert_char(char c) {
    if ( 'A' <= c && c <= 'Z')
        return c + 32; // 转换小写
    else if ( 'a' <= c && c <= 'z')
        return c - 32; // 转换大写
    else
        return c; // 其他不变
}

/**
 * 设置抽象事件
 * @param ev  抽象事件
 * @param fd  这里是  socket
 * @param events  关怀的事件类型，读事件还是写事件
 * @param call_back  回调函数
 * @param arg  指向自己的指针
 * @param index  g_events 下标号
 */
void eventSet(struct myevent_s *ev, int fd, int events, Callback_t call_back, void * arg, int index)
{
    ev->fd = fd;
    ev->call_back = call_back;
    ev->events = events;
    ev->arg = arg;
    ev->index = index;
}

/**
 * 从 epoll 中删除 监听事件
 * @param efd
 * @param ev
 */
void eventDel(int efd, struct myevent_s* ev)
{
    if(ev->status != 1)
        return ;
    epoll_ctl(efd, EPOLL_CTL_DEL, ev->fd, NULL);
    printf("epoll_ctl del [fd = %d] events[%0x] g_events[%d] success\n", ev->fd, ev->events, ev->index);
    ev->status = 0;
}

/**
 * 注册 抽象事件 到epoll
 * @param efd
 * @param ev
 */
void eventAdd(int efd, struct myevent_s* ev)
{
    struct epoll_event epv;
    bzero(&epv, sizeof epv);
    int op;
    epv.data.ptr = ev; // 抽象事件
    epv.events = ev->events; // 关心的类型
    /// status 记录自己是否在epoll树上
    if(ev->status == 1){
        op = EPOLL_CTL_MOD; // 修改文件描述符对应的事件;
    }
    else{ // 不在 就添加
        op = EPOLL_CTL_ADD;// 向 epoll 实例注册文件描述符对应的事件；
        ev->status = 1;
    }
    ///
    if(epoll_ctl(efd, op, ev->fd, &epv) < 0)
    {
        printf("epoll_ctl add failed\n");
    }
    else{
        printf("epoll_ctl add [fd = %d] events[%0x] g_events[%d] success\n", ev->fd, ev->events, ev->index);
    }
    return ;
}

/**
 * send 事件 触发 处理回调函数
 * @param fd
 * @param events
 * @param arg
 */
void senddata(int fd, int events, void *arg)
{
    struct myevent_s *ev = (struct myevent_s*) arg;
    assert(fd == ev->fd);
    printf("fd = %d\n", fd);
    /// 发送数据
    int nbytes = write(ev->fd, ev->buf, ev->len);
    printf("nbytes = %d\n", nbytes);
    eventDel(g_efd, ev); // 一次收发结束 删除  epoll 树上的 事件，不再监听
    if(nbytes > 0)
    {
        /// 设置 抽象事件 注册回调函数 并绑定到 epoll 事件多路分发器 （这里是 read事件）
        /// 回调函数 recvdata 处理 read事件
        eventSet(ev, ev->fd, EPOLLIN, recvdata, ev,ev->index);
        /// 由于之前删除了 需重新注册 将 事件 挂到 epoll 树上
        eventAdd(g_efd, ev);
    }
    else{
        close(ev->fd);
        printf("write error\n");
    }
}

/**
 * read 事件 触发 处理回调函数
 * @param fd
 * @param events
 * @param arg
 */
void recvdata(int fd, int events, void *arg)
{
    struct myevent_s *ev = (struct myevent_s*) arg;
    assert(fd == ev->fd);
    /// 接收数据
    int nbytes = read(fd, ev->buf, sizeof ev->buf);

    if(nbytes > 0)
    {
        /// nbytes > 0 表示实际从缓冲区读取的字节数目
        ev->len = nbytes;
        ev->buf[nbytes] = 0;
        for (int i = 0; i < nbytes; ++i) {
            ev->buf[i] = convert_char(ev->buf[i]);
        }
        printf("%s\n", ev->buf);
        /// 设置 抽象事件 注册回调函数 并绑定到 epoll 事件多路分发器 （这里是 send 事件）
        /// 回调函数 senddata 处理 send事件
        eventSet(ev, ev->fd, EPOLLOUT, senddata, ev,ev->index);
        eventAdd(g_efd, ev);
    }
    else if(nbytes == 0)
    {
        /// nbytes > 0 表示对端已经关闭了 socket，这时候也要关闭这个socket
        close(ev->fd); // close 会直接导致从关注列表remove
        printf("fd:%d closed \n", ev->fd);
        ev->status = 0;
    }
    else
    {
        /// 失败了
        printf("fd:%d recv error\n", ev->fd);
        ev->status = 0;
    }
    return ;
}

/**
 * accept 触发 处理回调函数
 * @param fd  套接字
 * @param events  事件类型
 * @param arg   指向抽象事件的指针
 */
void acceptConn(int fd, int events, void *arg)
{
    // 找一个空闲的event   MAX_EVENTS
    int i = 0;
    for(i = 0; i < MAX_EVENTS; ++i)
    {
        // 0 表示还没有挂在 epoll树上
        if(g_events[i].status == 0)
        {
            break;
        }
    }
    if(i == MAX_EVENTS)
    {
        printf("max events limited\n");
        return;
    }
    //设置套接字非阻塞
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd , F_SETFL , flags|O_NONBLOCK);

    struct sockaddr_storage ss;
    socklen_t slen = sizeof(ss);
    /// accept 创建 连接套接字
    int connfd = accept(fd, (struct sockaddr *) &ss, &slen);
    my_error(connfd, "accept error");
    /// 设置 抽象事件 注册回调函数 并绑定到 epoll 事件多路分发器 （这里是 read事件）
    /// 回调函数 recvdata 处理 read事件
    eventSet(&g_events[i], connfd, EPOLLIN, recvdata, &g_events[i],i);
    eventAdd(g_efd, &g_events[i]);
}

void initlistensocket(int epfd, unsigned short port)
{
    int lfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    my_error(lfd, "socket error");

    int reuse = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse);
    struct sockaddr_in addr;
    bzero(&addr, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int ret = bind(lfd, (struct sockaddr *) &addr, sizeof addr);
    my_error(ret, "bind error");

    ret = listen(lfd, 16);
    my_error(ret, "listen error");

    printf("fd= %d\n", lfd);
    /// 设置 抽象事件 注册回调函数 并绑定到 epoll 事件多路分发器 （这里是 accept 事件）
    /// accept 事件触发 注册的回调函数 acceptConn 处理
    /// 这里 使用 MAX_EVENTS 下标的抽象事件 作为 accept 事件
    eventSet(&g_events[MAX_EVENTS], lfd, EPOLLIN, acceptConn, &g_events[MAX_EVENTS],MAX_EVENTS);
    eventAdd(epfd, &g_events[MAX_EVENTS]);
}

int main()
{
    unsigned short port = 5555;
    /// 创建epoll文件描述符
    /// EPOLL_CLOEXEC标志与open 时的O_CLOEXEC 标志类似，即进程被替换时会关闭文件描述符
    g_efd = epoll_create1(EPOLL_CLOEXEC);
    my_error(g_efd, "epoll_create error");
    /// 创建监听套接字 并 注册 events
    initlistensocket(g_efd, port);
    struct epoll_event events[MAX_EVENTS+1];
    printf("server running:port[%d]\n", port);

    int checkpos = 0, i;
    while(1)
    {
        // 超时验证
        /*
        long now = time(NULL);
        for(i = 0; i < 100; i++, checkpos++)
        {
            if(checkpos == MAX_EVENTS)
                checkpos = 0;
            if(g_events[checkpos].status != 1)
                continue;

            long duration = now - g_events[checkpos].last_active;
            if(duration >= 60)
            {
                close(g_events[checkpos].fd);
                printf("[fd=%d] timeout\n", g_events[checkpos].fd);
                eventDel(g_efd, &g_events[checkpos]);
            }
        }
        */

        /// 等待内核 I/O 事件的分发
        int nfd = epoll_wait(g_efd, events, MAX_EVENTS + 1, 1000);
        my_error(nfd, "epoll_wait error");
        /// nfd 分发的事件的个数 这里遍历处理
        for(i = 0; i < nfd; ++i)
        {

            /// 抽象的事件 处理 监听 与 连接 套接字
            struct myevent_s *ev = (struct myevent_s *)events[i].data.ptr;
            /// 回调函数处理事件   回调函数 在 eventSet 注册
            if((events[i].events & EPOLLIN) && (ev->events & EPOLLIN))
                ev->call_back(ev->fd, events[i].events, ev->arg);
            if((events[i].events & EPOLLOUT) && (ev->events & EPOLLOUT))
                ev->call_back(ev->fd, events[i].events, ev->arg);
        }
    }
    return 0;
}
