#include <iostream>
#include <cstring>
#include "ThreadPool.hpp"

char convert_char(char c) {
    if ( 'A' <= c && c <= 'Z')
        return c + 32; // 转换小写
    else if ( 'a' <= c && c <= 'z')
        return c - 32; // 转换大写
    else
        return c; // 其他不变
}

void convert_string(const int lengh, char* buff ) {
    for (int i = 0; i < lengh; ++i) {
        buff[i] = convert_char(buff[i]);
    }
}
//// 测试任务封装
//int main() {
//    std::cout << "Hello, World!" << std::endl;
//    char test[1024] = "leacock";
//    int len = strlen(test);
//    Task t1(convert_string, len, test),
//    t2(convert_string, len, test);
//    t1.run();
//    std::cout << "t1.run() , test = " << test << std::endl;
//    t2.run();
//    std::cout << "t2.run() , test = " << test << std::endl;
//    return 0;
//}

// 测试任务封装
int main() {
    /// 队列可以自己实现
    ThreadPool<std::queue<Task *>> tp(5);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    char test1[24] = "leacock";
    char test2[24] = "ABCDEFG";
    char test3[24] = "1a2B3C4d";

    tp.addOneTask(new Task(convert_string, strlen(test1), test1));
    tp.addOneTask(new Task(convert_string, strlen(test2), test2));
    tp.addOneTask(new Task(convert_string, strlen(test3), test3));

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "convert_string test1 = " << test1 << std::endl;
    std::cout << "convert_string test2 = " << test2 << std::endl;
    std::cout << "convert_string test3 = " << test3 << std::endl;

    return 0;
}
