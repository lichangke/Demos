
#include <dirent.h>
#include <string>
#include <cstring>
#include <iostream>
#include <list>
#include <sys/stat.h>
#include <map>
#include <fstream>


void getFiles(const std::string& path, std::map<time_t,std::string,std::greater<time_t >> &filesMap)
{
    DIR *dir;
    struct dirent *ptr;

    if ((dir = opendir(path.c_str())) == nullptr)
    {
        perror("Open dir error...");
        exit(1);
    }

    // 确保路径最后有 /
    std::string newDir = path;
    char end = *(--newDir.end());  // 字符串最后一个字符
    if (end != '/') {
        newDir = newDir + "/";
    }

    // d_type表明该文件的类型：文件(8)、目录(4)、链接文件(10)
    while ((ptr = readdir(dir)) != nullptr)
    {
        struct stat buff{};
        if (std::strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0) {
            continue;
        } else if (ptr->d_type == 8) { // 文件(8)
            std::string fullPath = newDir + ptr->d_name;

            if (0==stat(fullPath.c_str(),&buff)) {
                time_t  time = buff.st_mtime; /// 最后一次修改文件的事件
                filesMap.insert(std::make_pair(time,fullPath));
            }
        } else if (ptr->d_type == 10) { // 链接文件(10)
            continue;
        } else if (ptr->d_type == 4) { // 目录(4)
            getFiles(newDir + ptr->d_name + "/", filesMap);
        }
    }
    closedir(dir);
}

int main ()
{
    std::string pathdir = "/home/leacock/CLionProjects";

    // 按time_t也就是key从大到小排列，也就是时间由近及远
    std::map<time_t,std::string,std::greater<time_t >> filesMap;
    filesMap.clear();
    getFiles(pathdir,filesMap);


    std::ofstream outfile("filesinfo.txt");


    auto iter = filesMap.begin();
    while(iter != filesMap.end()) {
        time_t time = iter->first;
        std::string filePath = iter->second;
        iter++;
        char buffer[255] = {0};
        tm *tm = localtime(&time);
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", tm);
        std::string tmp = buffer;
        std::string msg;
        msg.append(buffer).append(" time = ").append(std::to_string(time)).append(" dirPath = ").append(filePath);
        std::cout << msg << std::endl;
        outfile << msg << std::endl;
    }
    outfile.close();
    return 0;
}