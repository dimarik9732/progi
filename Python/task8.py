count = int(input())

for i in range(count):
    a, b, n = map(int, input().split(' '))
    s = a * n + (b * (n - 1) * n ) // 2
    print(s)
