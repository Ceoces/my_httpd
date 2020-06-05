# my_httpd
C++实现的httpd

### 编译
```make```

生成的文件保存在/path/to/project/bin中

### 配置文件

```
Address=0.0.0.0;  #绑定的地址
Port=2019;  #绑定的端口号
HostPath = /var/www/html; #网站根目录路径
```

### 运行

```
cd bin
./myhttpd
```
