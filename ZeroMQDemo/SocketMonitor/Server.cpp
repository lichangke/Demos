#include <iostream>
#include <zmq.h>
#include <thread>
#include <cstring>

class ZeroMQMonitorTest{

public:
    ZeroMQMonitorTest(){
        bOnRunning = false;
        /// 1、创建上下文
        context = zmq_ctx_new();
        /// 2、创建 socket
        socket = zmq_socket(context,ZMQ_DEALER);
        /// 3、如有需要设置 套接字选项
        int ivl = 0;
        // ZMQ_HANDSHAKE_IVL 设定最大握手间隔 这里0不超时失败,
        // 不同系统间通过zeromq通讯时，内部握手机制 在加密协议下会握手失败
        zmq_setsockopt(socket,ZMQ_HANDSHAKE_IVL,&ivl,sizeof(int));

    }
    ~ZeroMQMonitorTest() {
        bOnRunning = false;
        /// 关闭套接字、销毁上下文
        zmq_close (socket);
        zmq_ctx_destroy (context);
    }
    int start(){
        bOnRunning = true;
        /// 4、绑定 socket
        zmq_bind(socket, "tcp://0.0.0.0:5555");

        /// 创建线程 监听 套接字事件  ZMQ_EVENT_ALL 所有事件
        zmq_socket_monitor(socket, "inproc://monitor.inproc", ZMQ_EVENT_ALL);
        /// 监听线程
        std::thread monitorThread(&ZeroMQMonitorTest::socketMonitor,this);
        pthread_setname_np(monitorThread.native_handle(), "monitorThread");
        monitorThread.detach();

        /// 数据线程  用于 socket 数据通讯
        std::thread pollerThread(&ZeroMQMonitorTest::socketPoller,this);
        pthread_setname_np(pollerThread.native_handle(), "pollerThread");
        pollerThread.detach();
        return 0;
    }
    int stop() {
        bOnRunning = false;
        return 0;
    }

private:
    void *context; // ZeroMQ上下文
    void *socket; // ZeroMQ socket
    bool bOnRunning;

    void socketMonitor() {
        uint16_t event;  // 事件 number (16 bits)
        int32_t value;  // 事件 value (32 bits)
        std::cout << "starting monitor..." << std::endl;

        /// 为了收集 zmq_socket_monitor 监听的套接字事件
        /// 1、创建 ZMQ_PAIR 事件监听 套接字 2、连接到 zmq_socket_monitor 中的 监听地址  "inproc://monitor.inproc"
        /// 参看 http://api.zeromq.org/master:zmq-socket-monitor
        void *monitor = zmq_socket(context, ZMQ_PAIR);
        zmq_connect(monitor, "inproc://monitor.inproc");

        /* zmq_pollitem_t 结构体 用于 zmq_poll
        typedef struct
        {
            void //*socket//;
            int //fd//;
            short //events//;
            short //revents//;
        } zmq_pollitem_t;
        socket fd 同时设置 套接字优先
        revents 被先清除，后有事件来后被置位
        */
        zmq_pollitem_t items[2];
        items[0].socket = monitor;
        items[0].events = ZMQ_POLLIN ;
        /// ZMQ_POLLIN ： 从套接字接收至少一个消息而不阻塞
        /// ZMQ_POLLOUT : 至少可以向套接字发送一条消息而不阻塞。

        zmq_msg_t msg; // 消息结构体

        while(bOnRunning) {
            /// IO多路复用  1 表示使用 items 数组中的第一个
            /// timeout值是 -1，zmq_poll()进入无限阻塞等待状态，直到至少一个zmq_pollitem_t项上请求的事件发生了
            zmq_poll(items,1, 1000);

            if (!(items[0].revents & ZMQ_POLLIN)) {
                // 监听的事件为 ZMQ_POLLIN 应该进不来
                continue;
            }
            /// 接收消息帧
            /**
             * Each event is sent as two frames.
             * The first frame contains an event number (16 bits), and an event value (32 bits) that provides additional data according to the event number.
             * The second frame contains a string that specifies the affected endpoint.
             */
            /// 每个事件发送两帧数
            /// The first frame
            zmq_msg_init(&msg); /// 初始化msg引用的Message对象以表示空消息。
            zmq_msg_recv(&msg, monitor, 0); /// 从套接字接收消息部件

            /// zmq_msg_data 返回指向msg引用的消息对象的消息内容的指针。
            /// 永远不要直接访问zmq_msg_t成员，而是始终使用zmq_msg系列函数。
            const char *data = (const char *) zmq_msg_data(&msg);
            memcpy(&event, data, 2);  /// event number  16bits
            memcpy(&value, data + 2, 4);  /// event value  32bits
            zmq_msg_close(&msg); // 释放ZeroMQ消息

            /// The second frame
            zmq_msg_init(&msg);
            zmq_msg_recv(&msg, monitor, 0);

            int size = zmq_msg_size(&msg);
            data = (const char *)zmq_msg_data(&msg);
            char *address = new char[size + 1];
            memcpy(address, data, size);

            zmq_msg_close(&msg);

            switch (event){
                case ZMQ_EVENT_LISTENING:
                    std::cout << "listening socket address " << address << std::endl;
                    break;
                case ZMQ_EVENT_ACCEPTED:
                    std::cout << "accepted socket address " << address << std::endl;
                    break;
                case ZMQ_EVENT_CONNECTED:
                    std::cout << "connected socket address " << address << std::endl;
                    break;
                case ZMQ_EVENT_CLOSE_FAILED:
                    std::cout << "socket address " << address << std::endl;
                    break;
                case ZMQ_EVENT_CLOSED:
                    std::cout << "closed socket address " << address << std::endl;
                    break;
                case ZMQ_EVENT_DISCONNECTED:
                    std::cout << "disconnected socket address " << address << std::endl;
                    break;
                case ZMQ_EVENT_HANDSHAKE_FAILED_NO_DETAIL:
                    std::cout << "HANDSHAKE_FAILED " << address << std::endl;
                default:
                    break;
            }
            delete[] address;
            std::cout << "socket event value " << value << std::endl;
            std::cout << "socket event num " << event << std::endl;
        }
        zmq_close(monitor);
    }

    void socketPoller() {
        zmq_pollitem_t item;
        item.socket = socket;
        item.events = ZMQ_POLLIN;
        zmq_msg_t msg;
        while (bOnRunning) {
            zmq_poll(&item, 1, 1000);
            if(item.revents == ZMQ_POLLIN) {
                zmq_msg_init(&msg);
                zmq_msg_recv(&msg, socket, 0);

                /// zmq_msg_size 以字节为单位,检索消息内容大小
                /// 字符串并未包含结束符
                int size = zmq_msg_size(&msg);
                std::cout << "msg size = " << zmq_msg_size(&msg) << std::endl;
                const char *data = (const char *)zmq_msg_data(&msg);
                char *str = new char[size+1]{'\0'}; //初始化为 '\0'   zmq_msg_recv 接收不带结束符
                memcpy(str, data, size);
                std::cout << "Receive: \n" << str << std::endl;
                zmq_msg_close(&msg);

                //回传
                /// 初始化指定大小的zeromq消息
                zmq_msg_init_size(&msg, strlen(str));
                memcpy(zmq_msg_data(&msg), data, strlen(str));
                zmq_msg_send(&msg, socket, 0);
                delete[] str;
            }
        }
        int test = 1;
    }
};


int main() {
    std::cout << "Hello, World!" << std::endl;
    ZeroMQMonitorTest zeroMqMonitorTest;
    zeroMqMonitorTest.start();
    getchar();
    zeroMqMonitorTest.stop();
    return 0;
}
