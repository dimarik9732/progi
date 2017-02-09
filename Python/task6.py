n = int(input())
res = ""
for i in range(n):
    x = list(map(int,input().split()))
    y = x[0]/x[1]
    #print(y)
    if abs(y - int(y)) <= 0.5:
        y = int(y)
    elif y > 0:
        y = int(y) + 1
    else:
        y = int(y) - 1

    res =res + str(y) + " "
print(res)
