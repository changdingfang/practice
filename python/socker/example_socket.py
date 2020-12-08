#!/usr/python
# -*- coding:utf-8 -*-
import socket

# 建立进程对象
sk = socket.socket()

# 确定通信地址和端口
address = ('127.0.0.1',9082)

# 绑定地址，通过bind绑定
sk.bind(address)

# 监听连接信息。后面的参数数字代表最大等待数目，也就是允许几个客户端连接。如果服务端的个数超出这个个数，就会报错。
sk.listen(100)
print('waiting....')     # 提示等待连接中

f = open("test.txt", "ab+");
conn,addr = sk.accept()
cnt = 0
while True:
    mes = conn.recv(10240)
    cnt += 1
    print("[%d], len: [%d]" % (cnt, len(mes)))
    f.write(mes)

sk.close()
