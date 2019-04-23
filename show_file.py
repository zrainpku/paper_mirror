# -*- coding: utf-8 -*-
import time
import os
import sys
import re
import base64
import email


def read_file2(filename):
    f=open(filename,'r')
    msg=email.message_from_file(f)
    subject = msg.get("subject") 
    #h = email.Header.Header(subject)
    #dh = email.Header.decode_header(h)
    #subject = dh[0][0]
    print( "########################################################")
    print( "subject:", subject)
    print ("from: ", email.utils.parseaddr(msg.get("from"))[1]) 
    print ("to: ", email.utils.parseaddr(msg.get("to"))[1])


    for par in msg.walk():
        if not par.is_multipart(): 
            name = par.get_param("name")
            if name:
                #h = email.Header.Header(name)
                #dh = email.Header.decode_header(h)
                #fname = dh[0][0]
                print ('have 附件名:')
                continue
                data = par.get_payload(decode=True) 
                try:
                    fatt = open(fname, 'wb') 
                except:
                    print ('附件名有非法字符，自动换一个')
                    fatt = open('aaaa', 'wb')
                fatt.write(data)
                fatt.close()
            else:
                pagecont_src=par.get_payload(decode=True)
                pagecont=""
                try:   
                    pagecont=pagecont_src.decode(encoding='gbk')
                except:
                    print("不是GBK,使用utf-8解码 ")
                    try:
                        pagecont=pagecont_src.decode(encoding='utf-8')
                    except:
                        pagecont=pagecont_src.decode(encoding='gb2312')
                print(pagecont)
            print( '+'*60 )
    f.close()


def read_file(filename):
    f=open(filename,'r')
    flag=False
    for line in f.readlines():
        if len(line)<=0:
            print(" kong hang !!")
            continue
        if flag :
            print(str(base64.b64decode(line),"utf-8"))
            #print("base64:  ",line)
        elif re.search('Encoding: base64',line):
            flag=True
            print("base64: head====: ",line)
            continue
        else:
            print("not base 64!")

def find_path(filepath):
    files = os.listdir(filepath)
    for fi in files:
        fi_d = os.path.join(filepath,fi)            
        if os.path.isdir(fi_d):
            find_path(fi_d)                  
        else:
            file_name=os.path.join(filepath,fi_d)
            print(file_name)
            read_file2(file_name)
find_path(sys.argv[1])


