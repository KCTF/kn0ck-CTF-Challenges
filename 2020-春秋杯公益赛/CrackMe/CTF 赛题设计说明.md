# CrackMe

### [题目描述]：

```
I have a bad memory recently. Please help me find the file password.
```

### [题目考点]：
```
1. 标准AES算法识别
```

### [题目环境]：
```
1. Windows
```

### [题目制作过程]：
1. 编写 aes.c 代码，详细代码请从 “源码” 文件夹获取，
2. 加密正确flag获取密文
3. 读取用户输入并加密与密文比较

### [题目writeup]：

1. 使用IDA打开，通过提示字符串定位到验证函数，找到AES构造函数

![](http://img.5am3.com/5am3/img/20200223222927.png)



2. 可以识别出以上数据为AES算法S盒

3. 下图所示为密文，易找到key为“696368756e716975”，将其使用标准AES算法解密即可得到flag

![](http://img.5am3.com/5am3/img/20200223222935.png)

4. 如使用openssl命令解密
```
openssl aes-128-ecb -d -in encrypt.txt -out plain.txt -K 36393633363837353665373136393735 -nopad
```
