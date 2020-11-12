
//  Hello World client
#include <zmq.h>
#include <cstdio>
#include <iostream>
#include <cstring>

int main ()
{
    /// 创建上下文
    void *context = zmq_ctx_new();
    /// 创建ZeroMQ socket
    void *socket = zmq_socket(context,ZMQ_DEALER);
    int ivl = 0;
    zmq_setsockopt(socket,ZMQ_HANDSHAKE_IVL,&ivl,sizeof(int));
    std::cout << "输入连接的ip： " << std::endl;
    char *line = nullptr;
    size_t len = 0;
    ssize_t num;
    num = getline(&line, &len, stdin);
    std::string ip = line;
    ip[num-1] = ':'; //去掉获取字符串最后的 \n
    //TODO 健康检测，校验ip地址输入
    std::string tcpconnect = "tcp://" + ip + "5555";
    /// 连接
    zmq_connect (socket, tcpconnect.c_str());
    free(line);


    ///循环发送数据、接收数据  使用 zmq_msg_send zmq_msg_recv zmq_poll
    zmq_msg_t msg;
    zmq_pollitem_t item;
    item.socket = socket;
    item.events = ZMQ_POLLOUT;
    while (true) {
        zmq_poll(&item, 1, 1000);
        if(item.revents & ZMQ_POLLOUT) {
            std::cout << "Send:"  << std::endl;
            char * input = nullptr;;
            num = getline(&input, &len, stdin);
            /// 4.发送数据
            /// 初始化指定大小的zeromq消息
            zmq_msg_init_size(&msg, num-1);
            memcpy(zmq_msg_data(&msg), line, num-1);
            int ret = zmq_msg_send(&msg, socket, 0);
            free(input);
            input = nullptr;
            zmq_msg_close(&msg);

            // 5.接收回复数据
            zmq_msg_init(&msg);
            zmq_msg_recv(&msg, socket, 0);

            int size = zmq_msg_size(&msg);
            std::cout << "msg size = " << zmq_msg_size(&msg) << std::endl;
            const char *data = (const char *)zmq_msg_data(&msg);
            char *str = new char[size+1];
            memcpy(str, data, size);
            std::cout << "Receive: \n" << str << std::endl;
            zmq_msg_close(&msg);
        }



    }
//    ///循环发送数据、接收数据  使用 zmq_msg_send zmq_msg_recv
//    while (true) {
//        std::cout << "Send:"  << std::endl;
//        num = getline(&line, &len, stdin);
//        /// 4.发送数据
//        /// 初始化指定大小的zeromq消息
//        zmq_msg_init_size(&msg, num);
//        memcpy(zmq_msg_data(&msg), line, num-1);
//        int ret = zmq_msg_send(&msg, socket, 0);
//        free(line);
//        zmq_msg_close(&msg);
//        // 5.接收回复数据
//        zmq_msg_init(&msg);
//        zmq_msg_recv(&msg, socket, 0);
//
//        int size = zmq_msg_size(&msg);
//        std::cout << "msg size = " << zmq_msg_size(&msg) << std::endl;
//        const char *data = (const char *)zmq_msg_data(&msg);
//        char *str = new char[size+1];
//        memcpy(str, data, size);
//        std::cout << "Receive: \n" << str << std::endl;
//        zmq_msg_close(&msg);
//    }

//    ///循环发送数据、接收数据  使用 zmq_send zmq_recv
//    while(true) {
//        char buffer [255];
//        std::cout << "Send:"  << std::endl;
//        num = getline(&line, &len, stdin); // 包括\n
//        zmq_send (socket, line, num - 1, 0);
//        free(line);
//        // 5.接收回复数据
//        zmq_recv (socket, buffer, 255, 0);
//        std::cout << "Receive: \n" << buffer << std::endl;
//    }

    /// 关闭套接字、销毁上下文
    zmq_close (socket);
    zmq_ctx_destroy (context);

    return 0;

}
