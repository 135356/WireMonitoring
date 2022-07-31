# bb HTTP server
###### 一款Web服务器软件，它是高效率、轻量级、免安装的
> [发布区 Releases 有已经编译好的二进制文件解压即可完成部署](https://github.com/135356/bb-HTTP-server/releases)
### 视频演示(3.0.1)
> [bilibili.com/video/BV1wS4y1t7gH](https://www.bilibili.com/video/BV1wS4y1t7gH?share_source=copy_web&vd_source=238df0657244861dbcd6cf34d4a3b4da)
### 数据库基本信息配置文件
> */usr/local/bb/bb_mysql_orm_config.conf*
### 日志文件
> */usr/local/bb/bb.log*
### html文件路径
> */usr/local/bb/html*
### file上传文件路径
> */usr/local/bb/file*
### 依赖包
> 安装 MySQL 路径为: /usr/local/mysql
### 下载安装
> 发布区 [Releases](https://github.com/135356/bb-HTTP-server/releases) 下载: [bb_4.0.2_linux_x86_64.tar](https://github.com/135356/bb-HTTP-server/releases/download/4.0.2/bb_4.0.2_linux_x86_64.tar) 解压到 /usr/local/bb
#### 使用
> 打开路径: cd /usr/local/bb  
> 运行: ./bbhttp_run start&  
> 停止: ./bbhttp_run stop
### 源码
> 路由头文件: /include/api/Route.h  
> 路由源文件: /src/Route.cpp
>> 如果你会C++你只需要更改这个文件便可完成路由功能  
>> 否则你目前只能用它搭建静态网站
