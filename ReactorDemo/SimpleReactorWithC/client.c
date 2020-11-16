
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE     4096
#define SERV_PORT 5555

int main() {

    int sockfd;
    struct sockaddr_in servaddr;
    // 创建了一个本地套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror( "create socket failed");
    }

    // 初始化目标服务器端的地址, TCP 编程中，使用的是服务器的 IP 地址和端口作为目标
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);


    // 发起对目标套接字的 connect 调用
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("connect failed");
    }

    char send_line[MAXLINE];
    bzero(send_line, MAXLINE);
    char recv_line[MAXLINE];
    bzero(recv_line, MAXLINE);
    // 从标准输入中读取字符串，向服务器端发送
    while (1) {
        bzero(recv_line,MAXLINE); // 注意每次清空

        if (fgets(send_line, MAXLINE, stdin) == NULL)
            break;

        int nbytes = sizeof(send_line);
        if (send(sockfd, send_line, nbytes,0) != nbytes)
            perror("write error");

        bzero(recv_line, MAXLINE); // 注意每次清空
        if (recv(sockfd, recv_line, MAXLINE,0) == 0)
            perror("server terminated prematurely");

        fputs(recv_line, stdout);

    }

    exit(0);
}
