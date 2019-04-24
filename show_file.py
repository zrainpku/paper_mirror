# -*- coding: utf-8 -*-
import time
import os
import sys
import re
import base64
import email


def read_file3(filename):
    f=open(filename,'r')
    print ('打开了文件')
    try:
        msg=email.message_from_file(f)
    except:
        print ('打开email失败')
        f.close()
        return 
    print ('打开了email')
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
                print ('没有附件:')
                continue
                pagecont_src=par.get_payload(decode=True)
                pagecont=""
                print(pagecont)
            print( '+'*60 )
    f.close()

def url_filter(email_name,url_list):
    white_list = ['pku.edu.cn','szpku.edu.cn','pkusz.edu.cn','pkusz.cn','pkuaa.edu.cn','pkuef.org','pku.org.cn']
    white_list.append('cuhk.edu.hk')
    url_ans=[]
    flag = False
    for url in url_list:
        regular=re.compile('http[s]?://[.*?/]|[.*?$]')
        try:
            temp_url=re.findall(regular,url)
        except:
            print("re.findall() error!")
            continue
     
        short_url=temp_url[0]
        print("url: ",url)
        print("short_url: ",short_url)
        print("temp_url: ",temp_url)
        flag_is_white = False
        for ur in white_list:
            reg=re.compile(ur)
            if re.search(reg,short_url):
                # 当前url在白名单
                flag_is_white = True
                break
        if flag_is_white ==False:
            url_ans.append(url)
    if len(url_ans)>0:
        print("当前邮件包含白名单之外的url，如下所示：")
        print(url_ans)
    else:
        print("当前邮件链接都在白名单以内")


def read_file2(filename):
    f=open(filename,'r')
    try:
        msg=email.message_from_file(f)
    except:
        print ('打开email失败')
        f.close()
        return 
    print ('读写email成功')
    subject = msg.get("subject") 
    #h = email.Header.Header(subject)
    #dh = email.Header.decode_header(h)
    #subject = dh[0][0]
    print( "##########开始读写新的邮件#################")
    print( "subject:", subject)
    print ("from: ", email.utils.parseaddr(msg.get("from"))[1]) 
    print ("to: ", email.utils.parseaddr(msg.get("to"))[1])

    ans_url=[]
    for par in msg.walk():
        if not par.is_multipart(): 
            name = par.get_param("name")
            if name:
                #h = email.Header.Header(name)
                #dh = email.Header.decode_header(h)
                #fname = dh[0][0]
                print ('当前邮件含有附件:')
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
                    pagecont=pagecont_src.decode(encoding='gb2312')
                    print("############################是gb2312编码####### ")
                except:
                    try:
                        pagecont=pagecont_src.decode(encoding='GBK')
                        print("#####################是gbk编码###########")
                    except:
                        try:
                            pagecont=pagecont_src.decode(encoding='utf-8')
                            print("####################是utf-8编码##########")
                        except:
                            try:
                                pagecont=pagecont_src.decode(encoding='gb18030')
                                print("####################是gb18030编码##########")
                            except:
                                pagecont=pagecont_src
                #print(pagecont)
                #re.search('http',pagecont)
                #regular=re.compile('[a-zA-Z]+://[^\s]*[.com|.cn|.org|.net|.info]')
                regular=re.compile('http[s]?://(?:[a-zA-Z]|[0-9]|[$-_@.&+]|[!*\(\),]|(?:%[0-9a-fA-F][0-9a-fA-F]))+')
                try:
                    ans_url_tmp=re.findall(regular,pagecont)
                except:
                    ans_url_tmp=['https://pku.edu.cn']
                ans_url.extend(ans_url_tmp)
               # print(ans_url_tmp)
            print( '+'*60 )
    url_filter(filename,ans_url) 
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
            print("新邮件",file_name)
            time.sleep(2)
            read_file2(file_name)
find_path(sys.argv[1])


