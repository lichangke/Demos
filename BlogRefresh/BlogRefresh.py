import requests
import re
import time
import random
payload = ""
# 请求头
#headers = {
#    "Accept": "*/*",
#    "Accept-Encoding": "gzip, deflate, br",
#    "Accept-Language": "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3",
#    "Cookie": "l=AurqcPuigwQdnQv7WvAfCoR1OlrRQW7h; isg=BHp6mNB79CHqYXpVEiRteXyyyKNcg8YEwjgLqoRvCI3ddxqxbLtOFUBGwwOrZ3ad; thw=cn; cna=VsJQERAypn0CATrXFEIahcz8; t=0eed37629fe7ef5ec0b8ecb6cd3a3577; tracknick=tb830309_22; _cc_=UtASsssmfA%3D%3D; tg=0; ubn=p; ucn=unzbyun; x=e%3D1%26p%3D*%26s%3D0%26c%3D0%26f%3D0%26g%3D0%26t%3D0%26__ll%3D-1%26_ato%3D0; miid=981798063989731689; hng=CN%7Czh-CN%7CCNY%7C156; um=0712F33290AB8A6D01951C8161A2DF2CDC7C5278664EE3E02F8F6195B27229B88A7470FD7B89F7FACD43AD3E795C914CC2A8BEB1FA88729A3A74257D8EE4FBBC; enc=1UeyOeN0l7Fkx0yPu7l6BuiPkT%2BdSxE0EqUM26jcSMdi1LtYaZbjQCMj5dKU3P0qfGwJn8QqYXc6oJugH%2FhFRA%3D%3D; ali_ab=58.215.20.66.1516409089271.6; mt=ci%3D-1_1; cookie2=104f8fc9c13eb24c296768a50cabdd6e; _tb_token_=ee7e1e1e7dbe7; v=0",
#    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64;` rv:47.0) Gecko/20100101 Firefox/47.0"
#}
# 手机 Mozilla/5.0 (Linux; Android 8.0; Pixel 2 Build/OPD3.170816.012) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Mobile Safari/537.36
# 手机 Mozilla/5.0 (Linux; Android 5.0; SM-G900P Build/LRX21T) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Mobile Safari/537.36
# Win10  Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.25 Safari/537.36 Core/1.70.3861.400 QQBrowser/10.7.4313.400
# Win10  Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Safari/537.36

User_Agent_4 = "Mozilla/5.0 (Linux; Android 8.0; Pixel 2 Build/OPD3.170816.012) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Mobile Safari/537.36"
User_Agent_3 = "Mozilla/5.0 (Linux; Android 5.0; SM-G900P Build/LRX21T) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Mobile Safari/537.36"
User_Agent_2 = "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.25 Safari/537.36 Core/1.70.3861.400 QQBrowser/10.7.4313.400"
User_Agent_1 = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Safari/537.36"

User_Agent_list = [User_Agent_1,User_Agent_2,User_Agent_3,User_Agent_4]

headers = {
    "Accept": "*/*",
    "Accept-Encoding": "gzip, deflate, br",
    "Accept-Language": "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3",
    "User-Agent": ""
}
# 获得文章列表urls
def getUrls(url):

    # 发送请求
    resp = requests.request("GET", url, data=payload, headers=headers)
    #设置解码方式
    resp.encoding=resp.apparent_encoding
    #这里会用设置的解码方式解码
    html_source = resp.text
    # 正则表达式，取出网页中的url链接（一些寻找注入点的工具也是这么做出来的）
    urls = re.findall("https://[^>\";\']*\d",html_source)
    new_urls=[]
    for url in urls:
        if 'details' in url:
            if url not in new_urls:
                new_urls.append(url)
    return new_urls


# 这里替换成你要刷访问量博客的首页
url_list0 = [
            "https://blog.csdn.net/leacock1991/category_10688843.html",  # C++设计模式计划
            "https://blog.csdn.net/leacock1991/category_10752326.html",  # Go 相关
            "https://blog.csdn.net/leacock1991/category_10611734.html",  # 数据库 相关
            "https://blog.csdn.net/leacock1991/category_10087010.html",  # 趣谈Linux操作系统 学习
            "https://blog.csdn.net/leacock1991/category_10611738.html",  # Python 相关
            "https://blog.csdn.net/leacock1991/category_10670887.html",  # 转载搬运计划
            "https://blog.csdn.net/leacock1991/category_9396904.html",   # 思维导图
            "https://blog.csdn.net/leacock1991" # 首页
            ]
url_list1 = [
            "https://blog.csdn.net/leacock1991/article/details/112853343", #  从0开始学GO之目录
            "https://blog.csdn.net/leacock1991/article/details/111713017",  #   设计模式之模式概述(模式汇总)(C++实现)
            "https://blog.csdn.net/leacock1991/article/details/107738817",  #   《MySQL必知必会》学习笔记 目录
            "https://blog.csdn.net/leacock1991/article/details/107650096",  #   一步一步学linux操作系统 目录
            "https://blog.csdn.net/leacock1991/article/details/104665452",  #   【剑指offer-2】题目目录【C++版本】
            #"https://blog.csdn.net/leacock1991/article/details/101467248",  #   网络协议笔记 目录
            "https://blog.csdn.net/leacock1991/article/details/101467244",  #   Python 项目实践目录
            "https://blog.csdn.net/leacock1991/article/details/101467212",  #   Python基础进阶学习目录
            "https://blog.csdn.net/leacock1991/article/details/101467221",  #   SQL必知必会笔记目录
            "https://blog.csdn.net/leacock1991" # 首页
            ]
url_list2 = [
            "https://blog.csdn.net/leacock1991/category_10688843.html",  # C++设计模式计划
            "https://blog.csdn.net/leacock1991/category_10752326.html",  # Go 相关
            "https://blog.csdn.net/leacock1991/category_10611734.html",  # 数据库 相关
            "https://blog.csdn.net/leacock1991/category_10087010.html",  # 趣谈Linux操作系统 学习
            "https://blog.csdn.net/leacock1991/category_10611738.html",  # Python 相关
            "https://blog.csdn.net/leacock1991/category_10670887.html",  # 转载搬运计划
            "https://blog.csdn.net/leacock1991/category_9319783.html",  # 趣谈网络协议 学习
            "https://blog.csdn.net/leacock1991/category_9538790.html",  # 算法训练营总结
            "https://blog.csdn.net/leacock1991/category_9396932.html",  # Linux相关
            "https://blog.csdn.net/leacock1991/category_10586501.html",  # 不知道如何分类的分类
            "https://blog.csdn.net/leacock1991/category_9718166.html",  # 网络编程实战
            "https://blog.csdn.net/leacock1991/category_9641683.html",  # 分布式学习
            "https://blog.csdn.net/leacock1991/category_9396904.html"  # 思维导图
            ]
url_list3 = [
            "https://blog.csdn.net/leacock1991/category_9319783.html",  # 趣谈网络协议 学习
            "https://blog.csdn.net/leacock1991/category_9538790.html",  # 算法训练营总结
            "https://blog.csdn.net/leacock1991/category_9396932.html",  # Linux相关
            "https://blog.csdn.net/leacock1991/category_10586501.html",  # 不知道如何分类的分类
            "https://blog.csdn.net/leacock1991/category_9718166.html",  # 网络编程实战
            "https://blog.csdn.net/leacock1991/category_9641683.html",  # 分布式学习
            ]
url_list4 = [
            "https://blog.csdn.net/leacock1991/category_10688843.html",  # C++设计模式计划
            "https://blog.csdn.net/leacock1991/category_10752326.html",  # Go 相关
            "https://blog.csdn.net/leacock1991/category_10670887.html",  # 转载搬运计划
            "https://blog.csdn.net/leacock1991/category_9396904.html",  # 思维导图
            "https://blog.csdn.net/leacock1991" # 首页
            ]
# url_1 = getUrls("https://blog.csdn.net/leacock1991")  # 首页
url_list = url_list4

if __name__ == '__main__':
    links = set()
   
    for i in range(4):
        if i == 0:
            headers["User-Agent"] = User_Agent_list[random.randint(0, 1)]
            for url in url_list:
                urls = getUrls(url)
                for link in urls:
                    if 0 == link.find("https://blog.csdn.net/leacock1991/article/details/"):
                        if -1 != link.find("#comments"):
                            continue
                        links.add(link)
        print("============start=============", i)
        print("Total links = ", str(len(links)))
        for link in links:
            try:
                requests.request("GET", link, data=payload, headers=headers)
            except:
                print("get except")
                continue
            print(link, "Ok")
            time.sleep(random.randint(1, 6))
        print("============end=============", i)
        time.sleep(60*20)
    print("=======for=range(4)=end======")
