#include <iostream>
#include "macroDemo.h"


int main() {
    std::cout << "Hello, World!" << std::endl;
    PRINT(1)
    std::cout << "==========" << std::endl;
    PRINT(1, 2)
    std::cout << "==========" << std::endl;
    PRINT(1, 2, 3)
    //PRINT(1, 2, 3, 4) // 超过宏定义中参数个数
    return 0;
}
