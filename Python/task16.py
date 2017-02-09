def my_round(y):
    print(y)
    if abs(y - int(y)) < 0.5:
        y = int(y)
    elif y > 0:
        y = int(y) + 1
    else:
        y = int(y) - 1

    return y


count = int(input())

for i in range(count):
    x = list(map(int, input().split(' ')))
    print(x)
    j = 0
    sum = 0
    while x[j] != 0:
        sum += x[j]
        j += 1
  #  print(sum)
   # print(j)
    print(my_round(4040.5))
    print(' ')
