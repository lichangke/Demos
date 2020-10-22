#include <iostream>
//  json.hpp 唯一需要引入的文件
#include <nlohmann/json.hpp>
// 使用如下语句方便使用
using json = nlohmann::json;

int main() {

    /// _json_pointer
    json j_original = R"({
      "baz": ["one", "two", "three"],
      "foo": "bar"
    })"_json;

    std::cout << "json j_original = " << j_original << std::endl;

    // 通过json指针访问成员
    // 如果没有 会新增元素其值为null
    // eg: j_original["/baz/5"_json_pointer]  会在 增加元素 到 6 个
    auto var1 = j_original["/baz/1"_json_pointer]; // "two"
    auto var2 = j_original["/baz/2"_json_pointer]; // "three"
    auto var5 = j_original["/baz/5"_json_pointer]; // null
    std::cout << "var1 = " << var1 << std::endl;
    std::cout << "var2 = " << var2 << std::endl;
    std::cout << "var3= " << j_original["/baz/3"_json_pointer] << std::endl;
    std::cout << "var4 = " << j_original["/baz/4"_json_pointer] << std::endl;
    std::cout << "var5 = " << var5 << std::endl;
    std::cout << "=============" << std::endl;
    /// patch
    // 补丁操作
    //  "op": 指定操作   "path"：指定 key值  "value"：指定值
    json j_patch = R"([
      { "op": "replace", "path": "/baz", "value": "boo" },
      { "op": "add", "path": "/hello", "value": ["world"] },
      { "op": "remove", "path": "/foo"}
    ])"_json;

    // 运行补丁， 不会修改原数据，返回修改后的数据
    json j_result = j_original.patch(j_patch);
    std::cout << "json j_original = " << j_original << std::endl;
    std::cout << "json j_result = " << j_result << std::endl;
    // {
    //    "baz": "boo",
    //    "hello": ["world"]
    // }

    // 创建一个JSON Patch，以便通过调用修补程序函数将 源值(j_original) 更改为 目标值(j_result)
    // 故 source.patch(diff(source, target)) == target; 总是为 true
    auto op_patch = json::diff(j_result, j_original);
    std::cout << "op_patch = " << op_patch << std::endl;
    // [
    //   { "op":" replace", "path": "/baz", "value": ["one", "two", "three"] },
    //   { "op": "remove","path": "/hello" },
    //   { "op": "add", "path": "/foo", "value": "bar" }
    // ]
    std::cout << "=============" << std::endl;
    /// Merge Patch
    // 可以不是用_json_pointer而是使用 merge_patch 来修改元素
    // 根据 j_patch 的值，操作 原数据
    json j_document = R"({
      "a": "b",
      "c": {
        "d": "e",
        "f": "g"
      }
    })"_json;
    std::cout << "json j_document = " << j_document << std::endl;
    // a patch
    json j_patch2 = R"({
      "a":"z",
      "c": {
        "f": null
      }
    })"_json;
    std::cout << "json j_patch2 = " << j_patch2 << std::endl;
    j_document.merge_patch(j_patch);
    std::cout << "json j_document = " << j_document << std::endl;
    // {
    //  "a": "z",
    //  "c": {
    //    "d": "e"
    //  }
    // }
    std::cout << "=============" << std::endl;
    return 0;
}
