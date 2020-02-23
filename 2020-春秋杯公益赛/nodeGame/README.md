# node_game

最近水了俩国际赛，发现node的题目蛮有意思。恰逢永信举办公益赛，然后根据学习到的知识点。自己出了一道。

[详细分析见博客](https://blog.5am3.com/2020/02/11/ctf-node1/)

## 运行题目

```bash
./start.sh
```

## 解决

1. /source 获取源码。
2. 审计发现两个敏感接口。分别是`/core`和`/file_upload`，一个是ssrf，一个是文件上传
3. 通过ssrf构造文件上传。(详见：[通过拆分攻击实现的SSRF攻击](https://xz.aliyun.com/t/2894))
4. 构造payload,编写exp。(详见:[exp.py](./exp.py))





## 致谢

- [hackim-2020/split_second](https://github.com/nullcon/hackim-2020/tree/master/web/split_second)