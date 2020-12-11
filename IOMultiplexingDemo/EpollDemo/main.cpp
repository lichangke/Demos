#include <cstdio>
#include <fcntl.h>
#include <cassert>
#include <unistd.h>
#include <sys/epoll.h>

int main ()
{
    int keyboard;
    int ret,i;
    char c;

    // 键盘输入
    keyboard = open("/dev/tty",O_RDONLY | O_NONBLOCK);
    assert(keyboard>0);

    int epoll_fd=epoll_create(10);
    //step1: epoll开始工作之前 先把文件描述符纳入epoll监管

    ///  场景1
    struct epoll_event  listen_fd_0{};
    listen_fd_0.data.fd = 0;
    listen_fd_0.events = EPOLLIN;
    ret = epoll_ctl(epoll_fd,EPOLL_CTL_ADD,0,&listen_fd_0);

    ///  场景2
    struct epoll_event  listen_fd{};
    listen_fd.data.fd = keyboard;
    listen_fd.events = EPOLLIN;
    ret = epoll_ctl(epoll_fd,EPOLL_CTL_ADD,keyboard,&listen_fd);

    struct epoll_event ready_events[10];
    bool flag = true;
    while(flag)
    {
        //step2: epoll开始工作 阻塞的等待文件描述符就绪
        ret= epoll_wait(epoll_fd,ready_events,10,1000);
        if (ret == -1)
            perror("poll error");
        else if (ret) {
            /*第三步  poll完成工作  有套接字就绪或者时间超时返回*/
            //step3:epoll完成工作  看自己感兴趣的套接字是否就绪
            for(int i=0;i<ret;i++){
                if(ready_events[i].data.fd==keyboard){
                    printf("data.fd==keyboard\n");
                    i=read(keyboard,&c,1); // 读取数据
                    if('\n'==c)
                        continue;
                    if ('q'==c) {
                        flag = false;
                        break;
                    }
                    printf("the input is %c\n",c);
                } else if (ready_events[i].data.fd==0) {
                    printf("data.fd==0\n");
                    i=read(0,&c,1); // 读取数据
                    if('\n'==c)
                        continue;
                    if ('q'==c) {
                        flag = false;
                        break;
                    }
                    printf("the input is %c\n",c);
                }
            }
        }
        //time out when ret = 0
        //else printf("time out/n");
    }
}