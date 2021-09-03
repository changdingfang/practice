#!/usr/bin/python
# -*- coding: utf-8 -*-
import smtplib    
from email.mime.multipart import MIMEMultipart    
from email.mime.text import MIMEText    
from email.mime.image import MIMEImage 
from email.header import Header   
import requests
import time
smtpserver = 'smtp.163.com'
username = '15738938828@163.com'
password='fd245292011'
sender='15738938828@163.com'
receiver='245292011@qq.com'
#receiver=['XXX@126.com','XXX@qq.com']


subject = '星座运势'
#subject = '中文标题'
#subject=Header(subject, 'utf-8').encode()


#构造邮件对象
msg = MIMEMultipart('mixed') 
msg['Subject'] = subject
#msg['From'] = 'private_server@163.com <XXX@163.com>'
msg['From'] = '15738938828@163.com <15738938828@163.com>'
msg['To'] = '245292011@qq.com'
#收件人为多个收件人,通过join将列表转换为以;为间隔的字符串
#msg['To'] = ";".join(receiver) 
#msg['Date']='2018-9-23'


text = content #传入文本  
text_plain = MIMEText(text,'plain', 'utf-8')    
msg.attach(text_plain)  




smtp = smtplib.SMTP()    
smtp.connect('smtp.163.com')
#用set_debuglevel(1)可以打印出和SMTP服务器交互的所有信息。
print(smtp.set_debuglevel(1))  
smtp.login(username, password)    
smtp.sendmail(sender, receiver, msg.as_string())    
smtp.quit()


#def main(): #构建定时任务
	#while 1 :
		#run()
		#time.sleep(60)#设置脚本运行的时间间隔
