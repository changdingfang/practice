#!/usr/python
# -*- coding:utf-8 -*-

import socket
import json
import time

if __name__=="__main__":
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    conn = 'unix通信scoket管道绝对路径'
    try:
        sock.connect(conn)
    except:
        print("connect error")
        exit(-2)

    sock.send("url")
    result_json = sock.recv(256)
    sock.close()
    print(result_json)
