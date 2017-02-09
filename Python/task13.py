count = int(input())

for raw in input().split(' '):
    len_ = len(raw)
    raw = int(raw)
    s = 0
    while len_ != 0:
        s += raw % 10 * len_
        len_ -= 1
        raw = raw // 10
    print(s)
    
