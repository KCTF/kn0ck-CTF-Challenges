# document
### [题目描述]：
```
管理人员信息档案真的是件费心的工作，如果有个程序能帮我简化这项工作该多好啊！
```

### [题目考点]：
```
1. UAF
2. tcache
```

### [题目环境]：
```
1. ubuntu 19.04 LTS（更新到最新）
2. Xinetd + chroot
```

### [题目制作过程]：
1. 编写 pwn.c 代码，详细代码请从 “源码” 文件夹获取，
2. 编译方式在源代码第一行，
3. 编写 xinetd 配置文件，设置好参数
4. 编写 Dockerfile

### [题目writeup]：

1. 检查题目保护，发现开了全保护

![](http://img.5am3.com/5am3/img/20200222143818.png)

2. 使用IDA反编译二进制程序，在删除功能的函数里free后没有清空指针，存在UAF漏洞。

![](http://img.5am3.com/5am3/img/20200222143826.png)

3. 测试直接利用double free，发现报错为`free(): double free detected in tcache 2`，得知libc版本为glibc-2.29。

![](http://img.5am3.com/5am3/img/20200222143836.png)

4. 编写 exp.py ，获取flag。

