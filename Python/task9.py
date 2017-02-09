count = int(input())

for i in range(count):
    a, b, c = map(int, input().split(' '))

    if a > b + c:
        print('0')
    elif b > a + c:
        print('0')
    elif c > a + b:
        print('0')
    else: print('1')

    print(' ')
