#encoding:utf-8
#import os
From_file=open('Jane_Eyre.txt')
f=open('_jane_eyre.txt','w')
count=0
tmp=[]
for each_line in From_file:
    #print(type(each_line))  each_line 是字符类型
    Delstr=list(each_line)
    Delstr.pop()  #弹出n
    Delstr.pop()  #弹出\
    Delstr.append(',\\')
    tmp="".join(Delstr)
    print(tmp)
    f.writelines(tmp)
    count+=1
    tmp=[]
f.close()
From_file.close()
print('文件中总共有：%d行'%count)

########################################
## python get_file,py > _jane_eyre.txt##
