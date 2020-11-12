#include <cstdio>
#include <fcntl.h>
#include <cassert>
#include <unistd.h>
#include<poll.h>

int main ()
{
    int keyboard;
    int ret,i;
    char c;

    // 键盘输入
    keyboard = open("/dev/tty",O_RDONLY | O_NONBLOCK);
    assert(keyboard>0);

    /*第一步 poll开始监听之前要知道要监管哪些套接字*/
    struct pollfd fds[1];
    fds[0].fd= keyboard;
    fds[0].events=POLLIN;
    fds[0].revents=POLLIN;



    while(true)
    {
        /*第二步 poll开始工作 阻塞的轮询看监管的套接字是否就绪*/
        ret=poll(fds,1,5000);
        if (ret == -1)
            perror("poll error");
        else if (ret) {
            /*第三步  poll完成工作  有套接字就绪或者时间超时返回*/
            if(fds[0].revents==POLLIN) // 是否是 keyboard
            {
                i=read(keyboard,&c,1); // 读取数据
                if('\n'==c)
                    continue;
                if ('q'==c)
                    break;
                printf("the input is %c\n",c);
            }
        }
        //time out when ret = 0
        //else printf("time out/n");
    }
}