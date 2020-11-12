#include <ctime>
#include <cstdio>
#include <sys/types.h>
#include <fcntl.h>
#include <cassert>
#include <unistd.h>

int main ()
{
    int keyboard;
    int ret,i;
    char c;

    // 键盘输入
    keyboard = open("/dev/tty",O_RDONLY | O_NONBLOCK);
    assert(keyboard>0);

    fd_set readmask;
    fd_set allreads;// 描述符集合
    struct timeval timeout{};
    timeout.tv_sec=0; // 超时时间
    timeout.tv_usec=50;
    FD_ZERO(&allreads); // 初始化了一个描述符集合
    FD_SET(keyboard,&allreads);  // 将 keyboard 描述符 设置为1 待检测


    while(true)
    {

        //每次循环，重新设置待测试的描述符集合
        //因为 select 调用每次完成测试之后，内核都会修改描述符集合，通过修改完的描述符集合来和应用程序交互
        //应用程序使用 FD_ISSET 来对每个描述符进行判断，从而知道什么样的事件发生。
        readmask = allreads;
        // 注意 keyboard+1
        ret=select(keyboard+1,&readmask,NULL,NULL,&timeout);
        if (ret == -1)
            perror("select error");
        else if (ret) {
            if(FD_ISSET(keyboard,&readmask)) // 是否是 keyboard
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