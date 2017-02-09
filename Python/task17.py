count = int(input())

array = list(map(int, input().split(' ')))

s = 0
for i in array:
    s += i
    s *= 113
    s %= 10000007
print(s)
