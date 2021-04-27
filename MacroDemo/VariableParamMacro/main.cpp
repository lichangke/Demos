#include <iostream>


#define F(...) f(__VA_ARGS__)

int f(int a,int b,int c){
    return a+b+c;
}

int f(int a,int b){
    return a-b;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::cout<< F(1,2,3) <<std::endl; // 调用 int f(int a,int b,int c)
    std::cout<< F(1,2) <<std::endl; // 调用 int f(int a,int b)
    return 0;
}
