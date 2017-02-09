n = int(input())
print(n)
for i in range(n):
    s = 0
    a,b,c = map(int,input().split(' '))
    d = a*b + c
    while d != 0:
        s += d % 10
        d = d // 10
    print(s)
    print(' ')
