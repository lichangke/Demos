#include <iostream>
//  json.hpp 唯一需要引入的文件
#include <nlohmann/json.hpp>
// 使用如下语句方便使用
using json = nlohmann::json;

// 函数可参看 api ： https://nlohmann.github.io/json/api/basic_json/

int main() {

    // 使用 push_back 创建数组， 将给定元素Val附加到JSON数组的末尾
    json j;
    j.push_back("foo");
    j.push_back(1);
    j.push_back(true);
    std::cout << "json j = " << j << std::endl;
    // emplace_back 在j尾部 加入 根据传递的参数arg创建的JSON值
    j.emplace_back(1.78);
    std::cout << "json j = " << j << std::endl;

    // 迭代器遍历
    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        std::cout << *it << '\n';
    }
    std::cout << "=============="  << std::endl;

    // 遍历 j
    for (auto& element : j) {
        std::cout << element << '\n';
    }
    std::cout << "=============="  << std::endl;

    // 取值 设置值
    const auto tmp = j[0].get<std::string>();
    std::cout << "json j = " << j << std::endl;
    j[1] = 42;
    std::cout << "json j = " << j << std::endl;
    bool foo = j.at(2);
    std::cout << " j.at(2) = " << foo << std::endl;

    std::cout << "=============="  << std::endl;

    // 是否相等
    std::cout <<"j == \"[\\\"foo\\\", 42, true]\"_json : " << (j == "[\"foo\", 42, true]"_json) << std::endl;
    // size
    std::cout << "j.size() : " << j.size()  <<  std::endl;
    // 是否为空
    std::cout << "j.empty() : " << j.empty()  <<  std::endl;
    // 类型
    j.type(); //  返回类型
    auto name = j.type_name();  // 将类型名称作为字符串返回
    std::cout << "j.type() : " << name <<  std::endl;
    // 清空
    j.clear();
    std::cout << "json j = " << j <<  std::endl;

    std::cout << "=============="  << std::endl;

    // 方便的类型检查
    j.is_null();
    j.is_boolean();
    j.is_number();
    j.is_object();
    j.is_array();
    j.is_string();

    // 创建一个对象， 字典
    json o;
    o["foo"] = 23;
    o["bar"] = false;
    o["baz"] = 3.141;

    // 也可以通过 emplace 添加新元素
    o.emplace("weather", "sunny");

    std::cout << "json o = " << o <<  std::endl;

    // 迭代器遍历
    for (json::iterator it = o.begin(); it != o.end(); ++it) {
        std::cout << it.key() << " : " << it.value() << "\n";
    }
    std::cout << "=============="  << std::endl;
    for (auto& el : o.items()) {
        std::cout << el.key() << " : " << el.value() << "\n";
    }
    std::cout << "=============="  << std::endl;
    // 结构化绑定  (C++17)
    for (auto& [key, value] : o.items()) {
        std::cout << key << " : " << value << "\n";
    }
    std::cout << "=============="  << std::endl;
    // contains 查找是否包含 key 值
    if (o.contains("foo")) {
        std::cout << R"(o.contains("foo"))"  << std::endl;
    }
    // 通过  find
    if (o.find("foo") != o.end()) {
        std::cout << R"(o.find("foo"))"  << std::endl;
    }
    // 通过 count  返回 key 的个数
    int foo_present = o.count("foo"); // 1
    int fob_present = o.count("fob"); // 0
    // 删除 某个键值对
    o.erase("foo");
    std::cout << "json o = " << o <<  std::endl;

    return 0;
}
