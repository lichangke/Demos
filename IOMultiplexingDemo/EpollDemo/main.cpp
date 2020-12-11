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
    keyboard = open("/dev/tty",O_RDONLY | O_NONBLOCK); // /dev/tty
    assert(keyboard>0);

    //step1: epoll开始工作之前 先把文件描述符纳入epoll监管
    struct epoll_event  listen_fd{};
    listen_fd.data.fd = keyboard;
    listen_fd.events = EPOLLIN;
    int epoll_fd=epoll_create(10);
	
    //epoll_ctl(epoll_fd,EPOLL_CTL_ADD,0,&listen_fd);  // 0 是标准输入 stdin
	epoll_ctl(epoll_fd,EPOLL_CTL_ADD,keyboard,&listen_fd);
	

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
                    i=read(keyboard,&c,1); // 读取数据
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