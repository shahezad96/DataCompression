import sys



def main():
    argv = sys.argv

    if(len(argv)<2):
        print("usage: freq_count.py [file]")
        return
    fcount = {}
    for i in range(256):
        fcount[i] = 0;
    with open(argv[1],'rb') as f:
        while True:
            tmp = f.read(100)
            if tmp == b'':
                break
            #print(tmp)
            for c in tmp:
                #print((" char = '{}' %d".format(c))%c)
                #c = ord(c)
                fcount[c] += 1

    items = sorted(fcount.items(),key=lambda x:x[1],reverse=True)
    for key,value in items:
        print("{:<3} : {:<6} ".format(key,value))

    input()
    return    
    

if __name__=="__main__":
    main()
