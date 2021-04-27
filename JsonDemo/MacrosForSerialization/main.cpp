#include <iostream>
#include <iomanip>
#include "json.hpp"

using json = nlohmann::json;

class Address {

public:
    Address(){
        postcode = 1024;
        housenumber = 100;
        street = "this is a address";
    }
    ~Address() = default;
    int postcode;

public:
    void show(){
        std::cout << "postcode = "  << postcode << std::endl;
        std::cout << "housenumber = "  << housenumber << std::endl;
        std::cout << "street = "  << street << std::endl;
    }
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Address, street, housenumber, postcode)
    //NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Address, street, housenumber, postcode) // 存在 private 不能使用 这个宏

private:
    std::string street;
    int housenumber;

};

namespace ns {

    struct Student {
        int age{};
        int num{};
        std::string name;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Student, age, num, name)
}

int main() {
    Address add1;
    add1.postcode = 4096;
    json j1 = add1;
    std::cout << std::setw(4) << j1 <<std::endl;
    Address add2;
    add2 = j1;
    add2.show();

    ns::Student st1;
    st1.name = "张三";
    st1.num = 1;
    st1.age = 18;
    json j2 = st1;
    std::cout << std::setw(4) << j2 <<std::endl;
    ns::Student st2;
    st2 = j2;
    std::cout << "st2.name = "  << st2.name << std::endl;
    std::cout << "st2.num = "  << st2.num << std::endl;
    std::cout << "st2.age = "  << st2.age << std::endl;

    return 0;
}
