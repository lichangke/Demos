#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 simple.json 文件
{
  "ok":true,
  "height": 20.123,
  "width": 1000,
  "name": "test"
}
 */

int main() {

    json j; // json 对象
    std::ifstream jfile("simple.json"); // 流读取
    jfile >> j; // 文件流形式读取 json 文件， 并存为 j
    jfile.close();

    std::cout << "json j = " << j << std::endl;

    bool ok = j.at("ok");
    float height = j["height"];
    int width = j["width"];
    std::string name = j.at("name");

    std::cout << "ok = " << ok << std::endl;
    std::cout << "height = " << height << std::endl;
    std::cout << "width = " << width << std::endl;
    std::cout << "name = " << name << std::endl;


    return 0;
}
