#include <iostream>
//  json.hpp 唯一需要引入的文件
#include <nlohmann/json.hpp>
// 使用如下语句方便使用
using json = nlohmann::json;

int main() {

    /// 简单使用
    //创建一个空的结构
    json j1;
    // 数字 存储为 double
    j1["pi"] = 3.141;
    // Boolean 存储为 bool
    j1["happy"] = true;
    // 字符串 存储为 std::string
    j1["name"] = "Niels";
    // 空值 传入 nullptr
    j1["nothing"] = nullptr;
    // 直接加入其他对象
    j1["answer"]["everything"] = 42;
    // 加入一个  array 存储为 std::vector (使用初始化列表)
    j1["list"] = { 1, 0, 2 };
    // 添加其他对象 (使用键值对初始化列表)
    j1["object"] = { {"currency", "USD"}, {"value", 42.99} };
    // 直接形如json格式进行初始化
    json j2 = {
            {"pi", 3.141},
            {"happy", true},
            {"name", "Niels"},
            {"nothing", nullptr},
            {"answer", {
                           {"everything", 42}
                   }},
            {"list", {1, 0, 2}},
            {"object", {
                           {"currency", "USD"},
                         {"value", 42.99}
                   }}
    };
    std::cout << "json j1 = " << j1 << std::endl;
    std::cout << "json j2 = " << j2 << std::endl;

    ///可使用函数 json::array() 和 json::object() 等 显示表示
    // 空数组
    json empty_array_explicit = json::array();
    // 空 {} 对象
    json empty_object_implicit = json({});
    json empty_object_explicit = json::object();
    // 通过 key/value 键值对创建数组 [["currency", "USD"], ["value", 42.99]]
    json array_not_object = json::array({ {"currency", "USD"}, {"value", 42.99} });
    std::cout << "json empty_array_explicit = " << empty_array_explicit << std::endl;
    std::cout << "json empty_object_implicit = " << empty_object_implicit << std::endl;
    std::cout << "json empty_object_explicit = " << empty_object_explicit << std::endl;
    std::cout << "json array_not_object = " << array_not_object << std::endl;
    std::cout << "json array_not_object[?][?] = " << array_not_object[1] << std::endl;
    return 0;
}
