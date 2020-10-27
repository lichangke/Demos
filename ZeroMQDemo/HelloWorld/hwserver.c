//  Hello World server

#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

int main (void)
{

    int major, minor, patch;
    zmq_version(&major, &minor, &patch); // 获取当前使用的ZeroMQ的版本号
    printf("Current ZeroMQ version is %d.%d.%d\n", major, minor, patch);

    ///  1.创建上下文
    void *context = zmq_ctx_new ();   // 创建 0MQ上下文  线程安全的

    /// // 2.创建、绑定套接字
    // ZMQ_REP 服务使用ZMQ_REP类型的套接字来接收来自客户端的请求并向客户端发送回复
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, "tcp://*:5555");
    assert (rc == 0);

    /// 3.循环接收数据、发送数据
    while (1) {
        char buffer [10];
        /// 4.接收数据
        zmq_recv (responder, buffer, 10, 0);
        printf ("Received Hello\n");
        sleep (1);
        /// 5.回送数据
        zmq_send (responder, "World", 5, 0);
    }
    return 0;
}