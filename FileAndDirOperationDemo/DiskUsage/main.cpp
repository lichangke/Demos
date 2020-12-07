#include <sys/statfs.h>
#include <cstdio>
#include <cstdint>

#define KB 1024.0       // 2^10
#define MB 1048576.0    // 2^20
#define GB 1073741824.0 // 2^30

/*
struct statfs{
    long f_type;     //文件系统的类型
    long f_bsize;   //经优化后的传输块的大小
    long f_blocks;  //文件系统数据块总数
    long f_bfree;    //可用块数
    long f_bavail;   //普通用户能够获得的块数
    long f_files;      //文件结点总数
    long f_ffree;     //可用文件结点数
    fisd_t f_fsid;     //文件系统标识
    long f_namelen;  //文件名的最大长度
}
*/

int main()
{
        struct statfs diskInfo{};

        statfs("/home", &diskInfo);
        uint64_t blocksize = diskInfo.f_bsize;                   // 每个block里包含的字节数
        uint64_t totalsize = blocksize * diskInfo.f_blocks;      // 总的字节数，f_blocks为block的数目

        printf("块数：%lu\n",diskInfo.f_blocks);
        printf("Total_size = %lu B = %f KB = %f MB = %f GB\n",
                totalsize,
                totalsize / KB,
                totalsize / MB,
                totalsize / GB);

        uint64_t freeDisk = diskInfo.f_bfree * blocksize;       // 剩余空间的大小
        uint64_t availableDisk = diskInfo.f_bavail * blocksize; // 可用空间大小

        printf("Disk_free = %f MB = %f GB\n"
                        "Disk_available = %f MB = %f GB\n",
                freeDisk / MB,
                freeDisk / GB,
                availableDisk / MB,
                availableDisk / GB);

        return 0;
}