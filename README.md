# WireMonitoring
###### 电缆监控系统C++后端
> 发布区 Releases 有已经编译好的二进制文件解压即可完成部署
### 视频演示
> [bilibili.com/video/BV1wS4y1t7gH](https://www.bilibili.com/video/BV1wS4y1t7gH?share_source=copy_web&vd_source=238df0657244861dbcd6cf34d4a3b4da)
### 数据库基本信息配置文件
> */usr/local/bb/bb_mysql_orm_config.conf*
### 日志文件
> */usr/local/bb/bb.log*
### html文件路径
> */usr/local/bb/html/wire_monitoring*
### file上传文件路径
> */usr/local/bb/file/wire_monitoring*
### 安装OpenSSL
> 路径为 /usr/local/openssl
>> *示例*:  
>> wget https://www.openssl.org/source/openssl-1.1.1d.tar.gz  
>> tar -xf openssl-1.1.1d.tar.gz  
>> cd ~/openssl-1.1.1d  
>> mkdir /usr/local/openssl  
>> ./config --prefix=/usr/local/openssl --openssldir=/usr/local/openssl/ssl  
>> make && make install  
### 安装MySQL
> 路径为: /usr/local/mysql
### 开始安装
> 发布区 Releases 下载: bb_3.0.1_linux_x86_64.tar 解压到 /usr/local/bb
#### 使用
> 运行: /usr/local/bb/wire_monitoring_run start&  
> 停止: /usr/local/bb/wire_monitoring_run stop
### 源码解析
> 路由头文件: /include/api/Route.h  
> 路由源文件: /src/Route.cpp
>> 如果你会C++你只需要更改这个文件便可完成路由功能  
>> 否则你目前只能用它搭建静态网站
