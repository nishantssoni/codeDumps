#NISHANT SHEKHAR SONI
#Friday,December 21
#input: an Integer less or equal to 21 digits



wrd = {
         0:"",
         1:"one ",
         2:"two ",
         3:"three ",
         4:"four ",
         5:"five ",
         6:"six ",
         7:"seven ",
         8:"eight ",
         9:"nine ",
         10:"ten ",
         11:"eleven ",
         12:"twelve ",
         13:"thirteen ",
         14:"fourteen ",
         15:"fifteen ",
         16:"sixteen ",
         17:"seventeen ",
         18:"eighteen ",
         19:"nineteen ",
         20:"twenty" ,
         30:"thirty",
         40:"forty", 
         50:"fifty",
         60:"sixty",
         70:"seventy",
         80:"eighty",
         90:"ninty"
          
}

place ={
        1:"hundred ",
        2:"thousand ",
        3:"million ",
        4:"billion ",
        5:"trillion ",
        6:"quadrillion ",
        7:"quintillion "
}
"""
#reverse the string
def reverse(s):
       return s[::-1]"""


#make digits in group like [21,623,678]
def  main(x):
    li =(int(x)).__format__(',').split(',')
    return li
"""    x = reverse(x)
    st=""
    li=[]
    for i in range(len(x)):
        st+=x[i]
        if (i+1)%3==0:
            li.append(reverse(st))
            st=''
    else:
        if len(x)%3!=0:
            li.append(reverse(st))
    li.reverse()"""
    
        

#store the word fetches from wrd and place in st and then return 
def prt(num,length,last_index):
    value = True
    st=""
    x = num
    o=x%10   #onece
    x//=10
    t = x%10  #tence
    x//=10
    h = x%10  #hundred
  #  x//=10
    if num<20 :
        st= wrd[num]
       
        if h==0 and t==0 and o==0:
            value = False
    else:
        
        if h>0:
            st+=wrd[h]+place[1]
        if ((t*10+o)<20):
            st+=wrd[t*10+o]
        else:
            if t>0:
                st+=wrd[t*10]
                if o>0:
                    st+="-"
            if o>0:
                st+=wrd[o]
        
    
        
    if (last_index>1 and value):        
        st+=place[length]        
    return  st            





# start 
x = input()
myst=""
num=int(x)
print("your number is ::: ",int(x).__format__(','),"\n\n\n")

if num<0:
    myst="Negative "
    x = str(abs(num))
if num==0:
    myst="zero"


if len(x)>21:
    print('enter some smaller or equal  number lesser then 21 digits');
else:
    temp_list = main(x)
    temp_len = len(temp_list)
    
    for i in temp_list:
        myst+=prt(int(i),temp_len,temp_len)
        temp_len-=1

print(myst)
print("\n\n")
print("if you like it then hit thumbs up!!!")
        
    
