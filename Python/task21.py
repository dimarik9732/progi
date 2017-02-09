m, n = map(int, input().split(' '))

count = []

for i in range(n):
    count.append(0)

arr = list(map(int, input().split()))

for x in arr:
    count[x - 1] += 1

for y in count:
    print(y)
    print(' ')
