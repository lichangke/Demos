#include <iostream>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <fstream>

using json = nlohmann::json;

int main() {

    /// 字符串 序列化与反序列化
    // 通过添加 _json 后缀将字符串反序列化为json变量
    json j1 = "{ \"happy\": true, \"pi\": 3.141 }"_json;
    // 最好使用原始字符串
    auto j2 = R"({"happy": true,"pi": 3.141})"_json;
    // 使用 json::parse() 显示使用
    auto j3 = json::parse(R"({ "happy": true, "pi": 3.141 })");
    std::cout << "json j1" << j1 << std::endl;
    std::cout << "json j2" << j2 << std::endl;
    std::cout << "json j3" << j3 << std::endl;
    // 将json变量序列化为 字符串  .dump()返回最初存储的字符串值。 dump()  dump(4)  4 显示格式便于查看
    std::string s = j3.dump();    // {"happy":true,"pi":3.141}
    std::cout << "string s j3.dump()" << s << std::endl;
    std::cout << "string s j3.dump(4)" << j3.dump(4) << std::endl;
    /// 序列化和赋值之间的区别
    /// 库只支持UTF-8 存储具有不同编码的字符串时，调用dump()可能会引发异常
    // 将字符串存储在JSON值中
    json j_string = "this is a string";
    std::cout << "json j_string = " << j_string << std::endl;
    // 获取字符串值 j_string.get<std::string>()
    auto cpp_string = j_string.get<std::string>();
    std::cout << "j_string.get<std::string>() cpp_string = " << cpp_string << std::endl;
    // 获取字符串值 并存到 变量
    std::string cpp_string2;
    j_string.get_to(cpp_string2);

    // 获取序列化的值(显式JSON序列化)
    std::string serialized_string = j_string.dump();
    // 输出原始字符串
    std::cout << "original string: " << cpp_string << " == " << cpp_string2 << " == " << j_string.get<std::string>() << '\n';
    // 出书序列化值
    std::cout << "serialized value: " << j_string << " == " << serialized_string << std::endl;

    /// 从标准输入输出流中 序列化与反序列化
    // 从标准输入反序列化
    json j4;
    std::cout << "please input:" << std::endl;
    std::cin >> j4;
    // 序列化到标准输出
    std::cout << j4 << std::endl;
    // 格式化输出
    std::cout << std::setw(4) << j4 << std::endl;

    /// 从文件流中 序列化与反序列化
    // 读取一个json文件
    std::ifstream i("file.json");
    json j5;
    i >> j5;
    std::cout << "json j5 =  " <<  j5 << std::endl;
    // 将美化的JSON写入另一个文件
    std::ofstream o("pretty.json");
    o << std::setw(4) << j5 << std::endl;   // std::setw(4)  设置格式
    return 0;
}
