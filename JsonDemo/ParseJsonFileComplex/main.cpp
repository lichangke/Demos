#include <iostream>
//  json.hpp 唯一需要引入的文件
#include <nlohmann/json.hpp>
#include <fstream>
#include <iomanip>
// 使用如下语句方便使用
using json = nlohmann::json;
/*
 Complex.json 文件
 {
  "output": {
    "width": 720,
    "height": 1080,
    "frameRate": 20,
    "crf": 31
  },
  "tracks": [
    {
      "name": "t1",
      "pieces": [
        {
          "file": "x.mp4",
          "startTime": 2,
          "endTime": 6
        },
        {
          "file": "y.mp4",
          "startTime": 9,
          "endTime": 13
        }
      ]
    },
    {
      "name": "t2",
      "pieces": [
        {
          "file": "z.mp4",
          "startTime": 0,
          "endTime": 10
        }
      ]
    }
  ]
}
 */
///从这个 json 文件，可以看到，它包括两大部分 "output" 和 "tracks"。
/// "tracks" 为数组有两个元素，包括 string 的 name，和另一个结构体 "pieces" 的数组。
/// 可以定义 3个结构体 outputinfo  pieceinfo trackinfo

namespace jsonns {

    struct outputinfo {
        int width;
        int height;
        int frameRate;
        int crf;
    };

    struct pieceinfo {
        std::string  pathname;
        int     startTime{};
        int     endTime{};
    };

    struct trackinfo {
        std::string      name;
        pieceinfo   pieces[5];
        int         size{};
    };

    /// j： json文件中 output 部分 ， v： 结构体 outputinfo
    void from_json(const json& j, outputinfo& v) {
        j.at("width").get_to(v.width);
        j.at("height").get_to(v.height);
        j.at("frameRate").get_to(v.frameRate);
        j.at("crf").get_to(v.crf);
    }
    /// j： json文件中 pieces 部分 ， v： 结构体 pieceinfo
    void from_json(const json&j, pieceinfo &p) {
        j.at("file").get_to(p.pathname);
        j.at("startTime").get_to(p.startTime);
        j.at("endTime").get_to(p.endTime);
    }
    /// j： json文件中 tracks 部分 ， v： 结构体 trackinfo
    void from_json(const json&j, trackinfo &t) {
        j.at("name").get_to(t.name);
        /// 遍历 tracks 中 多个 pieces
        int len = j["pieces"].size();
        for(int i = 0; i < len; i++) {
            t.pieces[i] = j["pieces"][i];
        }
        t.size = j["pieces"].size();
    }
}

/**
 * 1、从 json文件读取为结构体
 * 2、将结构体按json格式存到文件
 * @return
 */
int main() {
    std::cout << "Hello, World!" << std::endl;

    /****** 从json文件中读取数据 并 存放为结构体******/
    json j;
    std::ifstream jfile("Complex.json");
    jfile >> j;
    jfile.close();
    std::cout << "json j = " << j << std::endl;

    jsonns::outputinfo vi = j.at("output");
    /// tilength json文件中key = "tracks"值的大小
    int tilength = j["tracks"].size();
    /// 创建 trackinfo 数组
    jsonns::trackinfo ti[tilength];
    /// 填充数据
    for (int i = 0; i < tilength; i++) {
        ti[i] = j["tracks"][i];
    }
    /****** 从json文件中读取数据 并 存放为结构体 end******/


    /******* 将结构体数据存成json对象 并 写入文件 ******/
    json j2;
    // "output" 数据
    j2["output"]["width"] = vi.width;
    j2["output"]["height"] = vi.height;
    j2["output"]["frameRate"] = vi.frameRate;
    j2["output"]["crf"] = vi.crf;
    // "tracks" 数据， 是个数组
    json tracks;
    for (int i = 0; i < tilength; i++) {
        json ttmp;
        ttmp["name"] = ti[i].name;
        int len = ti[i].size; // pieces 多少个
        // "pieces" 数据 是个数组
        json pieces;
        for (int k = 0; k < len; ++k) {
            json ptmp;
            ptmp["file"] = ti[i].pieces[k].pathname;
            ptmp["startTime"] =  ti[i].pieces[k].startTime;
            ptmp["endTime"] = ti[i].pieces[k].endTime;
            pieces.push_back(ptmp); // 是 数组 通过 push_back 添加
        }
        ttmp["pieces"] = pieces;
        tracks.push_back(ttmp); // 是 数组 通过 push_back 添加
    }

    j2["tracks"] = tracks;
    std::cout << "json j2 = " << j2 << std::endl;
    std::ofstream o("pretty.json");
    o << std::setw(4) << j2 << std::endl;   // std::setw(4)  设置格式
    o.close();
    /******* 将结构体数据存成json对象 并 写入文件 end******/

    return 0;

}
