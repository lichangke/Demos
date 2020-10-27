
//  Hello World client
#include <zmq.h>
#include <stdio.h>

int main (void)
{
    printf ("Connecting to hello world server...\n");
    /// 1、创建上下文
    void *context = zmq_ctx_new ();   // 创建 0MQ上下文  线程安全的

    /// 2.创建、绑定套接字
    // ZMQ_REQ  客户端使用ZMQ_REQ类型的套接字向服务发送请求并从服务接收答复
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

    ///3.循环发送数据、接收数据
    int request_nbr;
    for (request_nbr = 0; request_nbr != 10; request_nbr++) {
        char buffer [10];
        printf ("Sending Hello %d...\n", request_nbr);
        /// 4.发送数据
        zmq_send (requester, "Hello", 5, 0);
        // 5.接收回复数据
        zmq_recv (requester, buffer, 10, 0);
        printf ("Received World %d\n", request_nbr);
    }
    /// 6.关闭套接字、销毁上下文
    zmq_close (requester);
    zmq_ctx_destroy (context);
    return 0;
}